```
import os
import sys
current_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(current_dir)

from nuscenes.nuscenes import NuScenes
from nuscenes.utils.data_classes import Box, LidarPointCloud
from pyquaternion import Quaternion
import numpy as np
import cv2 
import os
import multiprocessing
from functools import partial

np.set_printoptions(precision=3, suppress=True)

save_folder = "./vis_camera_1115_merge"
lidar_save_folder = "./vis_lidar_1115_merge"

version = "trainval"
dataroot = "/defaultShare/tmpnfs/dataset/zm_radar/nuscenes_fmt_with_labels/24-09-20_24-10-29_000"

nuscenes = NuScenes(version='v1.0-{}'.format(version), dataroot=dataroot, verbose=False)

cameras = ['CAM_FRONT', 'CAM_FRONT_LEFT', 'CAM_FRONT_RIGHT', 'CAM_BACK_LEFT', 'CAM_BACK', 'CAM_BACK_RIGHT']

def to_matrix4x4_2(rotation, translation, inverse=True):
    output = np.eye(4)
    output[:3, :3] = rotation
    output[:3, 3]  = translation
    
    if inverse:
        output = np.linalg.inv(output)
    return output

def to_matrix4x4(m):
    output = np.eye(4)
    output[:3, :3] = m
    return output

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

color_map = {
    'CAR': (0, 255, 0),  # Green
    'VAN': (255, 0, 0),  # Blue
    'TRUCK': (0, 0, 255),  # Red
    'BUS': (255, 255, 0),  # Cyan
    'ULTRA_VEHICLE': (255, 0, 255),  # Magenta
    'CYCLIST': (0, 255, 255),  # Yellow
    'TRICYCLIST': (128, 0, 128),  # Purple
    'PEDESTRIAN': (255, 165, 0),  # Orange
    'ANIMAL': (0, 128, 128),  # Teal
    'UNKNOWN_MOVABLE': (128, 128, 0),  # Olive
    'ROAD_FENCE': (0, 128, 255),  # Light Blue
    'TRAFFIC_CONE': (255, 128, 0),  # Light Orange
    'WATER_FILED_BARRIER': (128, 0, 255),  # Light Purple
    'LIFTING_LEVERS': (0, 255, 128),  # Light Green
    'PILLAR': (128, 128, 128),  # Gray
    'OTHER_BLOCKS': (64, 64, 64),  # Dark Gray
}

camera_positions = {
    'CAM_FRONT_LEFT':  (0, 0, 1920, 1280),
    'CAM_FRONT':       (1920, 0, 1920, 1280),
    'CAM_FRONT_RIGHT': (1920 * 2, 0, 1920, 1280),
    'CAM_BACK_LEFT':   (0, 1280, 1920, 1280),
    'CAM_BACK':        (1920, 1280, 1920, 1280),
    'CAM_BACK_RIGHT':  (1920 * 2, 1280, 1920, 1280)
}

def process_sample(sample):
    canvas_width = 1920 * 3
    canvas_height = 1280 * 2
    canvas = np.zeros((canvas_height, canvas_width, 3), dtype=np.uint8)

    for camera in cameras:
        camera_sample_data = nuscenes.get('sample_data', sample['data'][camera])
        image_file         = os.path.join(nuscenes.dataroot, camera_sample_data["filename"])

        ego_pose           = nuscenes.get('ego_pose', camera_sample_data['ego_pose_token'])
        global_to_ego      = to_matrix4x4_2(Quaternion(ego_pose['rotation']).rotation_matrix, np.array(ego_pose['translation']))

        camera_sensor    = nuscenes.get('calibrated_sensor', camera_sample_data['calibrated_sensor_token']) 
        camera_intrinsic = to_matrix4x4(camera_sensor['camera_intrinsic'])

        ego_to_camera    = to_matrix4x4_2(Quaternion(camera_sensor['rotation']).rotation_matrix, np.array(camera_sensor['translation']))

        global_to_image  = camera_intrinsic @ ego_to_camera @ global_to_ego 
        
        image = cv2.imread(image_file)
        width, height = image.shape[1], image.shape[0]  
        
        for annotation_token in sample['anns']:
            instance = nuscenes.get('sample_annotation', annotation_token)
            category_name = instance['category_name']
            
            label = category_name.split('.')[1] if '.' in category_name else category_name
            label = label.upper()
            
            color = color_map.get(label, (255, 255, 255))
            
            box = Box(instance['translation'], instance['size'], Quaternion(instance['rotation']))
            corners = np.ones((4, 8))
            corners[:3, :] = box.corners() 
            corners = (global_to_image @ corners)[:3]
            
            if np.all(corners[2] <= 0):
                continue
            
            corners[:2] /= corners[[2]]
            corners = corners.T.astype(int)
            
            ix, iy = [0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7], [4, 5, 6, 7, 1, 2, 3, 0, 5, 6, 7, 4]
            for p0, p1 in zip(corners[ix], corners[iy]):
                if p0[2] <= 0 or p1[2] <= 0:
                    continue
                cv2.line(image, (p0[0], p0[1]), (p1[0], p1[1]), color, 2, 16)
            
            visible_corners = corners[corners[:, 2] > 0]
            if len(visible_corners) > 0:
                label_position = (min(visible_corners[:, 0]), min(visible_corners[:, 1]) - 10)
                cv2.putText(image, label, label_position, cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)
        
        image = cv2.resize(image, (1920, 1280))
        cv2.putText(image, camera, (20, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2, cv2.LINE_AA)

        x, y, w, h = camera_positions[camera]
        canvas[y:y+h, x:x+w] = image

    return canvas

def create_bev_image_with_predictions(point_cloud, pred_boxes, pred_labels, ego_pose):
    img_size_x, img_size_y = 2000, 1200
    scale_x, scale_y = 200 / img_size_x, 120 / img_size_y

    label_colors = color_map

    bev_image = np.zeros((img_size_y, img_size_x, 3), dtype=np.uint8)
    shift_x, shift_y = img_size_x // 2, img_size_y // 2

    ego_rotation = Quaternion(ego_pose['rotation']).rotation_matrix
    ego_translation = np.array(ego_pose['translation'])
    global_to_ego = np.eye(4)
    global_to_ego[:3, :3] = ego_rotation.T
    global_to_ego[:3, 3] = -ego_rotation.T @ ego_translation

    for point in point_cloud:
        x, y = point[:2]
        pixel_x, pixel_y = int(x / scale_x) + shift_x, shift_y - int(y / scale_y)
        if 0 <= pixel_x < img_size_x and 0 <= pixel_y < img_size_y:
            bev_image[pixel_y, pixel_x] = [255, 255, 255]

    for box, label in zip(pred_boxes, pred_labels):
        box_center = np.array(box[:3])
        box_center_homogeneous = np.append(box_center, 1)
        box_center_ego = (global_to_ego @ box_center_homogeneous)[:3]

        x, y, z = box_center_ego
        dy, dx, dz, rotation = box[3:7]
        rotation += np.pi
        if rotation > 2 * np.pi:
            rotation -= 2 * np.pi

        global_rotation = Quaternion(axis=[0, 0, 1], angle=rotation)
        ego_rotation_quat = Quaternion(ego_pose['rotation']).inverse * global_rotation
        rotation = ego_rotation_quat.yaw_pitch_roll[0]

        center_x, center_y = int(x / scale_x) + shift_x, shift_y - int(y / scale_y)

        if not (0 <= center_x < img_size_x and 0 <= center_y < img_size_y):
            continue

        dx, dy = dx / scale_x, dy / scale_y
        rect = ((center_x, center_y), (int(dx), int(dy)), np.degrees(-rotation))
        box_points = cv2.boxPoints(rect)
        box_points = np.intp(box_points)

        color = label_colors.get(label, (255, 255, 255))
        cv2.drawContours(bev_image, [box_points], 0, color, 2)

        head_start = box_points[0]
        head_end = box_points[1]
        cv2.line(bev_image, tuple(head_start), tuple(head_end), (0, 0, 255), thickness=2)

        label_position = (int(center_x), int(center_y))
        cv2.putText(bev_image, label, label_position, cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 1, cv2.LINE_AA)

    return bev_image

def get_base_filename(lidar_filename):
    # Remove the .pcd.bin extension and __LIDAR_TOP__ substring
    base_name = os.path.splitext(os.path.splitext(os.path.basename(lidar_filename))[0])[0]
    return base_name.replace('__LIDAR_TOP__', '_')

def process_single_sample(sample, save_folder, lidar_save_folder):
    # Get LIDAR filename for naming
    lidar_sample_data = nuscenes.get('sample_data', sample['data']['LIDAR_TOP'])
    base_filename = get_base_filename(lidar_sample_data['filename'])
    
    # Process camera images
    canvas = process_sample(sample)
    canvas = cv2.resize(canvas, (2880, 1280))
    cv2.imwrite(os.path.join(save_folder, f"{base_filename}.jpg"), canvas)

    # Process LiDAR data
    lidar_file = os.path.join(nuscenes.dataroot, lidar_sample_data['filename'])
    pc = LidarPointCloud.from_file(lidar_file)
    points = pc.points[:3, :].T

    pred_boxes = []
    pred_labels = []
    for annotation_token in sample['anns']:
        instance = nuscenes.get('sample_annotation', annotation_token)
        category_name = instance['category_name']
        label = category_name.split('.')[1] if '.' in category_name else category_name
        label = label.upper()

        box = Box(instance['translation'], instance['size'], Quaternion(instance['rotation']))
        pred_boxes.append(np.concatenate([box.center, box.wlh, [box.orientation.yaw_pitch_roll[0]]]))
        pred_labels.append(label)
    
    ego_pose = nuscenes.get('ego_pose', lidar_sample_data['ego_pose_token'])

    bev_image = create_bev_image_with_predictions(points, pred_boxes, pred_labels, ego_pose)
    cv2.imwrite(os.path.join(lidar_save_folder, f"{base_filename}_lidar.jpg"), bev_image)

    return base_filename

def visualize_samples(sample_range=None):
    os.makedirs(save_folder, exist_ok=True)
    os.makedirs(lidar_save_folder, exist_ok=True)

    samples = nuscenes.sample

    if sample_range is None:
        selected_samples = samples
    elif isinstance(sample_range, slice):
        selected_samples = samples[sample_range]
    elif isinstance(sample_range, int):
        if 0 <= sample_range < len(samples):
            selected_samples = [samples[sample_range]]
        else:
            raise ValueError(f"Sample index {sample_range} is out of range. Total samples: {len(samples)}")
    else:
        raise ValueError("Invalid sample_range. Use None, slice (start:end), or int.")

    num_processes = multiprocessing.cpu_count()
    pool = multiprocessing.Pool(processes=num_processes)

    process_func = partial(process_single_sample, save_folder=save_folder, lidar_save_folder=lidar_save_folder)
    
    for i, filename in enumerate(pool.imap_unordered(process_func, selected_samples)):
        print(f"Processed sample {i+1}/{len(selected_samples)}, filename: {filename}")

    pool.close()
    pool.join()

if __name__ == '__main__':
    visualize_samples()  # Process all samples
    # visualize_samples(10)  # Process only the sample at index 10
    # visualize_samples(slice(10, 20))  # Process samples from index 10 to 19 (inclusive)
    # visualize_samples(slice(None, 5))  # Process first 5 samples
```


以下是对这段代码的详细描述，包括功能、逻辑和每个部分的实现细节：

---

## **代码功能概述**
这段代码的主要任务是处理和可视化 **nuScenes** 数据集的多传感器数据，包括 **相机图像** 和 **LiDAR 点云**。处理流程分为以下几个部分：
1. **相机数据可视化**：将 3D 边界框（以物体类别标注）投影到图像中，并生成合成视图。
2. **LiDAR 数据可视化**：将点云和边界框生成鸟瞰图，并标注物体类别。
3. **多进程处理**：利用 CPU 并行化批量处理多个样本，提高处理效率。

---

## **实现细节**

### **1. nuScenes 数据集的加载**
- **`NuScenes` 类初始化**：
  ```python
  nuscenes = NuScenes(version='v1.0-{}'.format(version), dataroot=dataroot, verbose=False)
  ```
  - `version` 和 `dataroot` 指定了 nuScenes 数据的版本和根路径。
  - `verbose=False` 禁止加载数据时的详细日志。
  
- **关键数据结构**：
  - **样本（sample）**：代表一个时间点的完整场景，包含 LiDAR、相机数据的标注等。
  - **样本数据（sample_data）**：与单一传感器（如某个相机）的数据对应。
  - **标注（annotations）**：包含物体类别、边界框位姿（位置、大小、旋转）等信息。

---

### **2. 相机数据处理**
#### **边界框投影**
- **边界框的多次坐标变换**：
  1. **全局坐标到车体坐标**：
     ```python
     global_to_ego = to_matrix4x4_2(Quaternion(ego_pose['rotation']).rotation_matrix, np.array(ego_pose['translation']))
     ```
     利用车辆位姿信息（ego_pose 的旋转和平移）生成转换矩阵。
  2. **车体坐标到相机坐标**：
     ```python
     ego_to_camera = to_matrix4x4_2(Quaternion(camera_sensor['rotation']).rotation_matrix, np.array(camera_sensor['translation']))
     ```
     利用相机相对于车体的标定信息生成转换矩阵。
  3. **相机坐标到图像像素坐标**：
     ```python
     global_to_image = camera_intrinsic @ ego_to_camera @ global_to_ego
     ```
     将所有变换矩阵串联，最终将全局坐标投影到图像像素平面。

- **绘制 3D 边界框**：
  - **将 3D 边界框转换为 8 个角点**：
    ```python
    corners = np.ones((4, 8))
    corners[:3, :] = box.corners()
    corners = (global_to_image @ corners)[:3]
    ```
  - **对可见的点绘制边界线**：
    ```python
    if p0[2] > 0 and p1[2] > 0:  # 深度值必须为正
        cv2.line(image, (p0[0], p0[1]), (p1[0], p1[1]), color, 2, 16)
    ```

#### **相机视图拼接**
- 根据相机的预定义位置（如前、后、左等）将各个相机图像拼接成一个大画布：
  ```python
  canvas[y:y+h, x:x+w] = image
  ```

---

### **3. LiDAR 数据处理**
#### **鸟瞰图（BEV, Bird’s Eye View）生成**
- **点云投影**：
  LiDAR 点云数据通过全局到车体的变换矩阵进行旋转和平移：
  ```python
  global_to_ego = np.eye(4)
  global_to_ego[:3, :3] = ego_rotation.T
  global_to_ego[:3, 3] = -ego_rotation.T @ ego_translation
  ```
  然后以固定比例投影到鸟瞰图像素平面：
  ```python
  pixel_x, pixel_y = int(x / scale_x) + shift_x, shift_y - int(y / scale_y)
  ```

- **边界框绘制**：
  使用 OpenCV 的 `cv2.boxPoints` 和 `cv2.drawContours` 在鸟瞰图上绘制矩形表示物体的边界框：
  ```python
  rect = ((center_x, center_y), (int(dx), int(dy)), np.degrees(-rotation))
  box_points = cv2.boxPoints(rect)
  cv2.drawContours(bev_image, [box_points], 0, color, 2)
  ```

---

### **4. 文件保存与多进程**
- **文件保存**：
  - 每个样本的相机图像合成视图保存为 JPEG 文件：
    ```python
    cv2.imwrite(os.path.join(save_folder, f"{base_filename}.jpg"), canvas)
    ```
  - 每个样本的鸟瞰图保存为 JPEG 文件：
    ```python
    cv2.imwrite(os.path.join(lidar_save_folder, f"{base_filename}_lidar.jpg"), bev_image)
    ```

- **多进程处理**：
  - 利用 `multiprocessing` 模块分配任务：
    ```python
    pool = multiprocessing.Pool(processes=num_processes)
    for i, filename in enumerate(pool.imap_unordered(process_func, selected_samples)):
        print(f"Processed sample {i+1}/{len(selected_samples)}, filename: {filename}")
    ```
  - 每个进程独立处理一个样本，最终合并结果。

---

### **关键模块解释**
- **`color_map`**：定义了不同类别物体的颜色，用于绘制边界框。
- **`camera_positions`**：定义了每个相机图像在合成画布上的位置。

---

### **示例使用**
- **处理所有样本**：
  ```python
  visualize_samples()
  ```
- **处理特定范围样本（如第 10-19 个）**：
  ```python
  visualize_samples(slice(10, 20))
  ```

---

### **改进方向**
1. **错误处理**：捕获文件读取、矩阵计算等可能的异常。
2. **动态调整尺寸**：当前的固定图像尺寸可能无法适配所有数据，建议根据输入数据动态计算分辨率。
3. **分布式处理**：如果样本量巨大，考虑使用分布式系统（如 Spark 或 Dask）。
4. **优化点云渲染**：点云处理可用 GPU 加速库（如 CUDA 或 OpenGL）优化效率。