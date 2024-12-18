```
import json
import uuid
from typing import List, Dict
import os
from collections import defaultdict
import numpy as np
from pyquaternion import Quaternion
import torch
from abc import abstractmethod
import glob

def generate_token():
    return uuid.uuid4().hex

def load_detection_results(label_dir: str) -> Dict[str, List[Dict]]:
    results = {}
    for file_path in glob.glob(os.path.join(label_dir, '*.txt')):
        filename = os.path.basename(file_path)
        parts = filename.split('__')
        timestamp_part = parts[-1]
        timestamp = str(int(timestamp_part.split('.')[0]))
        
        with open(file_path, 'r') as f:
            lines = f.readlines()
        
        boxes = []
        labels = []
        scores = []
        classes = []
        
        for line in lines:
            parts = line.strip().split()
            class_name = parts[0]
            box = [float(x) for x in parts[1:8]]
            score = float(parts[8])
            
            boxes.append(box)
            labels.append(class_name)
            scores.append(score)
            classes.append(class_name)
        
        results[timestamp] = {
            'boxes': boxes,
            'labels': labels,
            'scores': scores,
            'classes': classes
        }
    
    return results

def create_category_json(label_map: Dict[int, str]) -> List[Dict]:
    categories = []
    for idx, name in label_map.items():
        categories.append({
            "token": generate_token(),
            "name": name,
            "description": name
        })
    return categories

def yaw_to_quaternion(yaw: float) -> List[float]:
    """
    Convert a yaw angle to a quaternion.
    :param yaw: Yaw angle in radians.
    :return: Quaternion as [w, x, y, z].
    """
    return Quaternion(axis=[0, 0, 1], angle=yaw).elements.tolist()

def transform_box_to_global(box, pose):
    """
    Transform a 3D bounding box from the LiDAR frame to the global frame using corners.
    :param box: [x, y, z, w, l, h, yaw]
    :param pose: [x, y, z, qw, qx, qy, qz] ego pose
    :return: [x, y, z, w, l, h, qw, qx, qy, qz] in global frame
    """
    x, y, z, w, l, h, yaw = box
    pose_x, pose_y, pose_z, pose_qw, pose_qx, pose_qy, pose_qz = pose

    # Create ego pose transformation
    ego_translation = np.array([pose_x, pose_y, pose_z])
    ego_rotation = Quaternion(pose_qw, pose_qx, pose_qy, pose_qz)

    # Transform center to global frame
    global_center = ego_rotation.rotate(np.array([x, y, z])) + ego_translation

    # Calculate new orientation (yaw) in global frame
    global_yaw = yaw + Quaternion(pose_qw, pose_qx, pose_qy, pose_qz).yaw_pitch_roll[0]

    # Convert yaw to quaternion
    global_quaternion = Quaternion(axis=[0, 0, 1], angle=global_yaw)
    qw, qx, qy, qz = global_quaternion.elements

    return [global_center[0], global_center[1], global_center[2], w, l, h, qw, qx, qy, qz]

def convert_to_sample_annotation_and_instance(detection_results: Dict[str, List[Dict]], sample_data_json: str, category_json: str, ego_pose_json: str) -> tuple[List[Dict], List[Dict]]:
    # Load sample_data.json
    with open(sample_data_json, 'r') as f:
        sample_data = json.load(f)
    
    # Load ego_pose.json
    with open(ego_pose_json, 'r') as f:
        ego_poses = json.load(f)
    
    # Load category.json
    with open(category_json, 'r') as f:
        categories = json.load(f)
    
    # Create mappings
    ego_pose_token_to_pose = {pose['token']: pose for pose in ego_poses}
    timestamp_to_sample_token = {}
    sample_token_to_ego_pose_token = {}
    
    for data in sample_data:
        if data['filename'].startswith('samples/LIDAR_TOP/'):
            timestamp_to_sample_token[str(data['timestamp'])] = data['sample_token']
            sample_token_to_ego_pose_token[data['sample_token']] = data['ego_pose_token']
    
    class_to_category_token = {cat['name']: cat['token'] for cat in categories}
    
    sample_annotations = []
    instances = defaultdict(lambda: {"sample_annotation_tokens": [], "category_token": "", "nbr_annotations": 0})
    
    for timestamp, detections in detection_results.items():
        sample_token = timestamp_to_sample_token.get(timestamp)
        if not sample_token:
            print(f"Warning: No matching sample_token found for timestamp {timestamp}")
            continue
        
        ego_pose_token = sample_token_to_ego_pose_token.get(sample_token)
        if not ego_pose_token:
            print(f"Warning: No matching ego_pose_token found for sample_token {sample_token}")
            assert False
        
        ego_pose = ego_pose_token_to_pose.get(ego_pose_token)
        if not ego_pose:
            print(f"Warning: No matching ego_pose found for ego_pose_token {ego_pose_token}")
            assert False
        
        ego_pose_data = [ego_pose['translation'][0], ego_pose['translation'][1], ego_pose['translation'][2],
                         ego_pose['rotation'][0], ego_pose['rotation'][1], ego_pose['rotation'][2], ego_pose['rotation'][3]]
        
        for i, (box, score, class_name) in enumerate(zip(detections['boxes'], detections['scores'], detections['classes'])):
            # Transform box to global frame
            global_box = transform_box_to_global(box, ego_pose_data)
            x, y, z, w, l, h, qw, qx, qy, qz = global_box
            
            instance_token = generate_token()
            annotation_token = generate_token()
            
            annotation = {
                "token": annotation_token,
                "sample_token": sample_token,
                "instance_token": instance_token,
                "visibility_token": "4",  # Assuming full visibility, adjust if needed
                "attribute_tokens": [],  # Empty for now, add attributes if available
                "translation": [x, y, z],
                "size": [w, l, h],
                "rotation": [qw, qx, qy, qz],  # Now using full quaternion
                "prev": "",
                "next": "",
                "num_lidar_pts": 0,  # Set to 0 as we don't have this information
                "num_radar_pts": 0,  # Set to 0 as we don't have this information
                "category_token": class_to_category_token.get(class_name, ""),
                "detection_name": class_name,
                "detection_score": score,
            }
            
            sample_annotations.append(annotation)
            
            # Update instance information
            instances[instance_token]["sample_annotation_tokens"].append(annotation_token)
            instances[instance_token]["category_token"] = class_to_category_token.get(class_name, "")
            instances[instance_token]["nbr_annotations"] += 1
    
    # Convert instances to list format
    instance_list = []
    for instance_token, instance_data in instances.items():
        instance_list.append({
            "token": instance_token,
            "category_token": instance_data["category_token"],
            "nbr_annotations": instance_data["nbr_annotations"],
            "first_annotation_token": instance_data["sample_annotation_tokens"][0],
            "last_annotation_token": instance_data["sample_annotation_tokens"][-1]
        })
    
    return sample_annotations, instance_list

def main():
    label_dir = "/defaultShare/tmpnfs/dataset/zm_radar/label_data/24-09-20_11-00-00_000/labelTxt_output_241108"
    nuscenes_dir = "/defaultShare/tmpnfs/dataset/zm_radar/nuscenes_fmt_with_labels/24-09-20_10-00-00_000"
    
    # label_dir = "/defaultShare/tmpnfs/dataset/zm_radar/label_data/24-10-29_10-00-00_000/labelTxt_output2"
    # nuscenes_dir = "/defaultShare/tmpnfs/dataset/zm_radar/nuscenes_fmt_with_labels/24-10-29_10-00-00_000"
    
    sample_data_json_path = os.path.join(nuscenes_dir, 'v1.0-trainval/sample_data.json')
    ego_pose_json_path = os.path.join(nuscenes_dir, 'v1.0-trainval/ego_pose.json')
    sample_annotation_output_path = os.path.join(nuscenes_dir, 'v1.0-trainval/sample_annotation.json')
    instance_output_path = os.path.join(nuscenes_dir, 'v1.0-trainval/instance.json')
    category_json_path = os.path.join(nuscenes_dir, 'v1.0-trainval/category.json')
    
    label_map = {
        0: 'CAR',
        1: 'VAN',
        2: 'TRUCK',
        3: 'BUS',
        4: 'ULTRA_VEHICLE',
        5: 'CYCLIST',
        6: 'TRICYCLIST',
        7: 'PEDESTRIAN',
        8: 'ANIMAL',
        9: 'UNKNOWN_MOVABLE',
        10: 'ROAD_FENCE',
        11: 'TRAFFIC_CONE',
        12: 'WATER_FILED_BARRIER',
        13: 'LIFTING_LEVERS',
        14: 'PILLAR',
        15: 'OTHER_BLOCKS',
    }
    
    for path in [sample_annotation_output_path, instance_output_path, category_json_path]:
        if os.path.exists(path):
            os.remove(path)
    
    # Create and save category.json
    categories = create_category_json(label_map)
    with open(category_json_path, 'w') as f:
        json.dump(categories, f, indent=2)
    
    detection_results = load_detection_results(label_dir)
    sample_annotations, instances = convert_to_sample_annotation_and_instance(detection_results, sample_data_json_path, category_json_path, ego_pose_json_path)
    
    with open(sample_annotation_output_path, 'w') as f:
        json.dump(sample_annotations, f, indent=2)
    
    with open(instance_output_path, 'w') as f:
        json.dump(instances, f, indent=2)
    
    print(f"Converted {len(sample_annotations)} annotations. Saved to {sample_annotation_output_path}")
    print(f"Generated {len(instances)} instances. Saved to {instance_output_path}")
    print(f"Generated category.json with {len(categories)} categories. Saved to {category_json_path}")

if __name__ == "__main__":
    main()
```

这段代码是一个将检测结果转换为 **nuScenes** 格式的工具脚本，主要功能包括加载检测结果、将其转换为全球坐标系并生成与 nuScenes 数据格式兼容的 `sample_annotation` 和 `instance` JSON 文件，同时生成类别定义文件 `category.json`。以下是该脚本的功能解读和工作流程：

---

### **主要功能**
1. **生成唯一标识符 (`generate_token`)**:
   - 利用 `uuid4` 方法生成独特的标识符，用于标注对象和实例的 `token`。

2. **加载检测结果 (`load_detection_results`)**:
   - 从给定的标签目录中读取检测结果（`*.txt` 文件）。
   - 将检测框、类别、置信度分数等信息按时间戳组织到字典中。

3. **生成类别信息 (`create_category_json`)**:
   - 根据类别映射表 (`label_map`) 生成 nuScenes 标准格式的 `category.json`。

4. **转换方向角为四元数 (`yaw_to_quaternion`)**:
   - 将 yaw (偏航角) 转换为四元数，用于 3D 坐标系中表示物体的方向。

5. **坐标系转换 (`transform_box_to_global`)**:
   - 将检测框从 LiDAR 局部坐标系转换为全球坐标系，考虑 ego pose（自车位置和方向）的影响。

6. **生成 `sample_annotation` 和 `instance` 数据**:
   - 将每个检测结果关联到样本和实例，最终按照 nuScenes 数据结构生成 `sample_annotation.json` 和 `instance.json` 文件。

7. **主函数 (`main`)**:
   - 通过配置路径加载输入文件。
   - 处理检测结果，保存转换后的输出。

---

### **脚本执行流程**
1. **路径配置**:
   - 设置标签文件夹 (`label_dir`) 和 nuScenes 数据目录 (`nuscenes_dir`) 的路径。

2. **清理旧文件**:
   - 删除输出目录中已存在的 `sample_annotation.json`、`instance.json` 和 `category.json`。

3. **生成类别文件**:
   - 解析类别映射，生成 `category.json` 文件。

4. **加载检测结果**:
   - 从标签目录加载物体检测框、类别和分数。

5. **转换为 nuScenes 格式**:
   - 读取 `sample_data.json` 和 `ego_pose.json`，完成坐标转换。
   - 根据类别匹配生成 `sample_annotation` 和 `instance` 数据。

6. **保存文件**:
   - 输出生成的 `sample_annotation.json` 和 `instance.json` 文件。

7. **打印总结**:
   - 输出处理结果的统计信息。

---

### **数据文件描述**
- **输入文件**:
  - 标签目录：存储检测框 (`.txt` 文件)。
  - `sample_data.json`：描述样本的时间戳、文件路径等。
  - `ego_pose.json`：记录自车的位置信息。

- **输出文件**:
  - `category.json`：类别定义文件。
  - `sample_annotation.json`：检测结果转化的注释。
  - `instance.json`：物体实例信息。

---

### **关键代码说明**
#### **1. 检测框数据加载**
```python
for line in lines:
    parts = line.strip().split()
    class_name = parts[0]
    box = [float(x) for x in parts[1:8]]
    score = float(parts[8])
    boxes.append(box)
    labels.append(class_name)
    scores.append(score)
```
- 提取检测框数据，包括 `[x, y, z, w, l, h, yaw]` 和分类。

#### **2. 坐标转换**
```python
global_center = ego_rotation.rotate(np.array([x, y, z])) + ego_translation
global_quaternion = Quaternion(axis=[0, 0, 1], angle=global_yaw)
```
- 通过自车位姿将检测框从局部坐标系转换为全球坐标系。
- 计算全球坐标的旋转四元数。

#### **3. 生成 `sample_annotation`**
```python
annotation = {
    "token": annotation_token,
    "sample_token": sample_token,
    "instance_token": instance_token,
    ...
    "translation": [x, y, z],
    "size": [w, l, h],
    "rotation": [qw, qx, qy, qz],
    "category_token": class_to_category_token.get(class_name, ""),
    "detection_name": class_name,
    "detection_score": score,
}
sample_annotations.append(annotation)
```
- 将检测框信息保存为 nuScenes 格式的 `sample_annotation`。

#### **4. 实例记录**
```python
instances[instance_token]["sample_annotation_tokens"].append(annotation_token)
instances[instance_token]["nbr_annotations"] += 1
```
- 跟踪每个实例的注释记录，生成完整实例信息。

---

### **输出统计**
```python
print(f"Converted {len(sample_annotations)} annotations.")
print(f"Generated {len(instances)} instances.")
print(f"Generated category.json with {len(categories)} categories.")
```
- 打印转换和生成文件的统计信息，便于调试和验证。