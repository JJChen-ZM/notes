```
class NuScenesDatasetMerger:
    def __init__(self, source_dirs: List[str], output_dir: str):
        """
        Initialize the NuScenes dataset merger
        
        Args:
            source_dirs: List of paths to source nuScenes format datasets
            output_dir: Path to the output merged dataset
        """
        self.source_dirs = [Path(d) for d in source_dirs]
        self.output_dir = Path(output_dir)
        self.version = 'v1.0-trainval'
        
    def merge_json_files(self):
        """Merge all JSON files from the datasets"""
        print("Merging JSON files...")
        
        # List of JSON files to process
        json_files = [
            'attribute.json',
            'calibrated_sensor.json', 
            'category.json',
            'ego_pose.json',
            'instance.json',
            'log.json',
            'map.json',
            'sample.json',
            'sample_annotation.json',
            'sample_data.json',
            'scene.json',
            'sensor.json',
            'visibility.json'
        ]
        
        for json_file in json_files:
            print(f"Processing {json_file}...")
            merged_data = []
            seen_tokens: Dict[str, Dict[str, Any]] = {}  # 用于存储已见过的token及其对应的完整记录
            
            # Read and merge data from all sources
            for source_dir in self.source_dirs:
                json_path = source_dir / self.version / json_file
                if not json_path.exists():
                    print(f"Warning: {json_path} not found, skipping...")
                    continue
                    
                with open(json_path) as f:
                    data = json.load(f)
                
                # 处理每条记录
                for record in data:
                    token = record['token']
                    
                    if token in seen_tokens:
                        # 如果token已存在，检查内容是否完全相同
                        existing_record = seen_tokens[token]
                        if existing_record != record:
                            raise ValueError(
                                f"Conflict detected in {json_file}!\n"
                                f"Token {token} has different content in different source datasets:\n"
                                f"Existing: {existing_record}\n"
                                f"New: {record}\n"
                                f"This might indicate a problem with the dataset generation process."
                            )
                        # print(f"Skipping duplicate record with token {token} in {json_file}")
                        continue
                    
                    # 新token，添加到结果中并记录
                    seen_tokens[token] = record
                    merged_data.append(record)
            
            # Save merged file
            output_path = self.output_dir / self.version / json_file
            output_path.parent.mkdir(parents=True, exist_ok=True)
            with open(output_path, 'w') as f:
                json.dump(merged_data, f, indent=2)
                
            print(f"Merged {len(merged_data)} records in {json_file}")
    def merge_sensor_data(self):
        """Merge sensor data (samples and sweeps directories)"""
        print("Merging sensor data...")
        
        # Collect all files that need to be copied
        copy_tasks = []
        seen_files = {}  # 改为字典，存储文件名到完整路径的映射
        
        for source_dir in self.source_dirs:
            for data_type in ['samples', 'sweeps']:
                src_base = source_dir / data_type
                if not src_base.exists():
                    continue
                    
                for sensor_type in os.listdir(src_base):
                    src_sensor = src_base / sensor_type
                    if not src_sensor.is_dir():
                        continue
                        
                    dst_sensor = self.output_dir / data_type / sensor_type
                    for file_name in os.listdir(src_sensor):
                        src_file = src_sensor / file_name
                        dst_file = dst_sensor / file_name
                        
                        # 构建相对路径作为键，用于确保相同类型的文件进行比较
                        relative_path = f"{data_type}/{sensor_type}/{file_name}"
                        
                        if relative_path in seen_files:
                            # 如果文件已存在，检查内容是否相同
                            existing_file = seen_files[relative_path]
                            if os.path.getsize(src_file) != os.path.getsize(existing_file):
                                print("\nERROR: File size mismatch detected!")
                                print(f"File: {file_name}")
                                print(f"Type: {data_type}/{sensor_type}")
                                print(f"New file: {src_file}")
                                print(f"Size: {os.path.getsize(src_file)} bytes")
                                print(f"Existing file: {existing_file}")
                                print(f"Size: {os.path.getsize(existing_file)} bytes")
                                print("\nAborting merge process...")
                                exit(1)
                            # print(f"Skipping duplicate file {relative_path}")
                            continue
                            
                        seen_files[relative_path] = src_file
                        copy_tasks.append((src_file, dst_file))
        
        # Copy files using multiple threads
        with concurrent.futures.ThreadPoolExecutor(max_workers=16) as executor:
            list(tqdm(executor.map(self._copy_file, copy_tasks), 
                    total=len(copy_tasks),
                    desc="Copying sensor files"))
        
        print(f"Copied {len(copy_tasks)} files")

    def _copy_file(self, args):
        """Helper function to create symlink instead of copying file"""
        src, dst = args
        if not dst.parent.exists():
            dst.parent.mkdir(parents=True, exist_ok=True)
        
        # 如果src是软链接，获取其实际指向的路径
        if os.path.islink(src):
            real_path = os.path.realpath(src)
            os.symlink(real_path, dst)
        else:
            # 如果src不是软链接，直接链接到源文件
            os.symlink(src, dst)
    
    def copy_maps(self):
        """Copy maps directory from the first source (assuming they're identical)"""
        print("Copying maps directory...")
        maps_path = self.source_dirs[0] / 'maps'
        if maps_path.exists() and not (self.output_dir / 'maps').exists():
            shutil.copytree(maps_path, self.output_dir / 'maps')
    
    def merge(self):
        """Perform the complete merge operation"""
        print(f"Starting merge of {len(self.source_dirs)} datasets into {self.output_dir}")
        
        # Create output directory
        self.output_dir.mkdir(parents=True, exist_ok=True)
        
        # Merge all components
        self.merge_json_files()
        self.merge_sensor_data()
        self.copy_maps()
        
        print("Merge completed successfully!")
```

以下是该实现的功能和结构的中文翻译和详细说明：

### 功能和实现细节

1. **初始化 (`__init__`)**
   - 接受一个源目录列表和一个目标输出目录。
   - 使用 Python 的 `Path` 库为版本和源目录准备路径。

2. **合并 JSON 文件 (`merge_json_files`)**
   - 处理一个预定义的 NuScenes 格式 JSON 文件列表。
   - 读取和合并数据，确保不会添加重复的 `token`。
   - 验证数据完整性：如果某个 `token` 在多个数据集中存在，但记录内容不一致，会抛出错误以防止数据合并的不一致性。

3. **合并传感器数据 (`merge_sensor_data`)**
   - 处理二进制传感器数据的合并（例如 LiDAR 点云和摄像头图片）。
   - 通过相对路径和文件大小识别重复文件，避免不必要的重复。
   - 使用 `concurrent.futures` 和 `tqdm` 进行多线程文件操作，效率高并提供进度条显示。
   - 使用符号链接代替直接复制文件，节省磁盘空间并确保数据一致性。

4. **复制地图数据 (`copy_maps`)**
   - 假设地图数据在各个数据集中相同，仅从第一个源目录中复制一次。

5. **辅助函数**
   - `_copy_file`: 用于管理文件符号链接的创建。
   - 能够优雅地处理符号链接和标准文件。

6. **合并控制流程 (`merge`)**
   - 在合并前确保目标目录存在。
   - 依次调用各个合并组件（`merge_json_files`、`merge_sensor_data`、`copy_maps`）。
   - 提供详细的日志和用户反馈。

### 鲜明特点：稳健性和高效性

- **冲突检测**：确保合并过程中，如果某个 `token` 对应的 JSON 记录内容不一致，会给出有意义的错误信息并停止操作。
- **内存高效**：通过记录已处理的 `token` 和文件，最小化不必要的操作。
- **文件一致性**：使用符号链接避免文件重复，节省存储空间，并确保所有引用指向相同的数据。
- **并行处理**：通过多线程提升文件操作的速度。
- **扩展性**：模块化设计便于未来扩展，例如支持新的数据集组件或处理额外的数据格式。

### 潜在改进点

- **日志记录**：添加结构化日志记录，便于大规模数据集合并的可追溯性。
- **自定义冲突解决**：提供解决冲突的选项（例如优先使用某个特定数据集的记录）。
- **验证机制**：实现合并后的验证步骤，确保所有数据正确链接并可访问。
- **错误恢复**：引入机制以支持中断的合并过程，尤其适用于大规模数据集。

### 应用场景

该脚本非常适合于合并自动驾驶或机器人领域的大规模数据集。它在保持数据一致性和完整性的同时，通过符号链接有效利用存储资源，适用于需要处理大量传感器数据的场景。





```
Traceback (most recent call last):
  File "/defaultShare/tmpnfs/dataset/zm_radar/code/dataset_prep/s08_merge_multi_nuscenes_datasets/01_merge_datasets.py", line 214, in <module>
    main()
  File "/defaultShare/tmpnfs/dataset/zm_radar/code/dataset_prep/s08_merge_multi_nuscenes_datasets/01_merge_datasets.py", line 210, in main
    merger.merge()
  File "/defaultShare/tmpnfs/dataset/zm_radar/code/dataset_prep/s08_merge_multi_nuscenes_datasets/01_merge_datasets.py", line 176, in merge
    self.merge_sensor_data()
  File "/defaultShare/tmpnfs/dataset/zm_radar/code/dataset_prep/s08_merge_multi_nuscenes_datasets/01_merge_datasets.py", line 140, in merge_sensor_data
    list(tqdm(executor.map(self._copy_file, copy_tasks), 
  File "/root/miniconda3/envs/torch110/lib/python3.9/site-packages/tqdm/std.py", line 1181, in __iter__
    for obj in iterable:
  File "/root/miniconda3/envs/torch110/lib/python3.9/concurrent/futures/_base.py", line 609, in result_iterator
    yield fs.pop().result()
  File "/root/miniconda3/envs/torch110/lib/python3.9/concurrent/futures/_base.py", line 439, in result
    return self.__get_result()
  File "/root/miniconda3/envs/torch110/lib/python3.9/concurrent/futures/_base.py", line 391, in __get_result
    raise self._exception
  File "/root/miniconda3/envs/torch110/lib/python3.9/concurrent/futures/thread.py", line 58, in run
    result = self.fn(*self.args, **self.kwargs)
  File "/defaultShare/tmpnfs/dataset/zm_radar/code/dataset_prep/s08_merge_multi_nuscenes_datasets/01_merge_datasets.py", line 155, in _copy_file
    os.symlink(real_path, dst)
FileExistsError: [Errno 17] File exists: '/defaultShare/tmpnfs/dataset/zm_radar/nuscenes_fmt_with_labels/24-09-20_00-00-00_000/nuscenes/samples/LIDAR_TOP/LIDAR_TOP__6335699598.pcd.bin' -> '/defaultShare/tmpnfs/dataset/zm_radar/nuscenes_fmt_with_labels/24-09-20_24-10-29_000/samples/LIDAR_TOP/24-09-20_10-00-00_000__LIDAR_TOP__06335699598.pcd.bin'

```

这个错误是因为在调用 `os.symlink` 创建符号链接时，目标文件已经存在。`FileExistsError` 的原因可能是目标路径 `dst` 在先前的操作中已经创建了同名的文件或符号链接。

---

### **解决思路**
1. **检查目标文件是否存在**：  
   在创建符号链接之前，检查目标文件路径是否已经存在。
   
2. **删除或跳过现有文件**：  
   如果目标文件已经存在，可以选择以下策略：
   - **覆盖文件**：删除现有文件，然后重新创建符号链接。
   - **跳过文件**：如果内容一致，则跳过当前操作。

3. **输出日志**：  
   对冲突情况输出详细日志，便于调试和分析。

---

### **修复代码**
修改 `_copy_file` 函数，添加文件存在检查和处理逻辑：

```python
def _copy_file(self, task):
    src, dst = task
    
    # 确保目标目录存在
    dst.parent.mkdir(parents=True, exist_ok=True)
    
    # 检查目标文件是否已存在
    if dst.exists():
        # 如果目标文件已存在，检查是否为符号链接且指向相同的源文件
        if dst.is_symlink() and os.readlink(dst) == str(src):
            print(f"Skipping existing symlink: {dst}")
            return
        else:
            # 删除现有文件或链接
            print(f"Removing existing file/link: {dst}")
            dst.unlink()
    
    # 创建符号链接
    try:
        os.symlink(src, dst)
    except Exception as e:
        print(f"Error creating symlink from {src} to {dst}: {e}")
        raise
```

---

### **关键点解析**
1. **`dst.exists()`**：
   - 检查目标路径是否已经存在。
   
2. **`dst.is_symlink()`**：
   - 如果目标是符号链接，使用 `os.readlink(dst)` 检查其目标地址是否与 `src` 一致。

3. **`dst.unlink()`**：
   - 删除现有文件或链接，确保新创建的符号链接不会被冲突影响。

4. **日志输出**：
   - 对跳过、删除等情况添加日志，便于后续排查。

---

### **防御性优化**
- **多线程竞争问题**：
  在高并发场景下，多个线程可能同时检查并试图操作同一文件。可以通过添加文件锁或使用临时中间文件缓解此问题。
  
- **确保数据一致性**：
  如果文件不是符号链接，而是普通文件或其他类型的对象，应额外检查其内容是否与源文件一致。

---

### **验证修复**
运行代码前，建议对以下情况进行测试：
1. **目标文件不存在**：应正常创建符号链接。
2. **目标文件已存在且一致**：应跳过操作。
3. **目标文件已存在但不一致**：应覆盖或输出错误。





## 完善 merge_sensor_data 函数的代码

下面是完善后的 `merge_sensor_data` 函数，增加了以下功能和优化：

### **优化点**
1. **目标文件检查**：在创建符号链接之前，检查目标文件是否存在，并根据情况处理。
2. **日志输出**：对跳过、覆盖、错误等情况输出详细日志。
3. **多线程安全**：避免多线程竞争问题。
4. **异常处理**：增加了更全面的异常捕获和输出。
5. **性能改进**：减少不必要的文件操作。

---

### **完善的 `merge_sensor_data` 函数**
```python
def merge_sensor_data(self):
    """Merge sensor data (samples and sweeps directories)"""
    print("Merging sensor data...")

    # Collect all files that need to be copied
    copy_tasks = []
    seen_files = {}  # Dictionary to store file paths for conflict detection

    for source_dir in self.source_dirs:
        for data_type in ['samples', 'sweeps']:
            src_base = source_dir / data_type
            if not src_base.exists():
                continue
            
            for sensor_type in os.listdir(src_base):
                src_sensor = src_base / sensor_type
                if not src_sensor.is_dir():
                    continue

                dst_sensor = self.output_dir / data_type / sensor_type
                for file_name in os.listdir(src_sensor):
                    src_file = src_sensor / file_name
                    dst_file = dst_sensor / file_name

                    # Construct a unique key for the file
                    relative_path = f"{data_type}/{sensor_type}/{file_name}"

                    # Check for duplicates
                    if relative_path in seen_files:
                        existing_file = seen_files[relative_path]
                        if os.path.getsize(src_file) != os.path.getsize(existing_file):
                            print("\nERROR: File size mismatch detected!")
                            print(f"File: {file_name}")
                            print(f"Type: {data_type}/{sensor_type}")
                            print(f"New file: {src_file}")
                            print(f"Size: {os.path.getsize(src_file)} bytes")
                            print(f"Existing file: {existing_file}")
                            print(f"Size: {os.path.getsize(existing_file)} bytes")
                            print("\nAborting merge process...")
                            exit(1)
                        # Skip duplicate file with matching size
                        continue

                    # Store the file path for later comparison
                    seen_files[relative_path] = src_file
                    copy_tasks.append((src_file, dst_file))

    # Copy files using multiple threads
    def safe_copy(task):
        src, dst = task

        # Ensure destination directory exists
        dst.parent.mkdir(parents=True, exist_ok=True)

        # Handle existing destination files
        if dst.exists():
            if dst.is_symlink() and os.readlink(dst) == str(src):
                # Skip if the symlink already points to the correct source
                return "skipped", src, dst
            else:
                # Remove existing file or symlink
                dst.unlink()

        try:
            # Create symbolic link
            os.symlink(src, dst)
            return "linked", src, dst
        except Exception as e:
            return "error", src, dst, str(e)

    print(f"Preparing to copy {len(copy_tasks)} files...")
    with concurrent.futures.ThreadPoolExecutor(max_workers=64) as executor:
        results = list(tqdm(executor.map(safe_copy, copy_tasks), 
                            total=len(copy_tasks), 
                            desc="Copying sensor files"))

    # Process results
    success_count = 0
    skip_count = 0
    error_count = 0

    for result in results:
        if result[0] == "linked":
            success_count += 1
        elif result[0] == "skipped":
            skip_count += 1
        elif result[0] == "error":
            error_count += 1
            src, dst, error_msg = result[1:]
            print(f"Error linking {src} to {dst}: {error_msg}")

    # Final summary
    print(f"\nMerge process completed:")
    print(f"  Successfully linked: {success_count} files")
    print(f"  Skipped existing links: {skip_count} files")
    print(f"  Errors: {error_count} files")

    if error_count > 0:
        print("Some files failed to merge. Please review the logs for details.")
```

---

### **功能说明**
1. **文件去重与冲突检查**：
   - 使用 `seen_files` 字典记录文件路径，防止重复。
   - 如果文件大小不同，终止合并，输出冲突信息。

2. **目标文件检查**：
   - 如果目标文件已存在：
     - 是符号链接且指向相同文件，则跳过。
     - 否则删除旧文件，创建新的符号链接。

3. **多线程处理**：
   - 使用 `concurrent.futures.ThreadPoolExecutor` 并行处理文件拷贝，提高性能。

4. **异常处理**：
   - 捕获 `os.symlink` 过程中可能发生的异常，并记录失败原因。

5. **进度条显示**：
   - 使用 `tqdm` 显示文件拷贝进度。

6. **结果汇总**：
   - 统计成功、跳过和失败的文件数，输出详细日志。

---

### **使用场景**
运行此函数时，确保以下路径有效：
1. `self.source_dirs`：源目录列表，包含 `samples` 和 `sweeps` 数据。
2. `self.output_dir`：目标合并目录。

根据任务量和硬件性能，可以调整 `max_workers` 的值优化多线程性能。