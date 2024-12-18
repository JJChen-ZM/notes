## 利用多线程运行 du -sh ./ 命令
使用多线程运行 `du -sh ./` 命令，可以加速对多个目录或路径计算磁盘占用的任务。以下是一个 Python 实现，使用 `concurrent.futures.ThreadPoolExecutor` 来实现多线程调度。

---

### **代码示例：多线程运行 `du -sh`**

```python
import os
import subprocess
from concurrent.futures import ThreadPoolExecutor, as_completed

def get_disk_usage(path):
    """
    调用 du -sh 命令计算目录占用
    """
    try:
        result = subprocess.run(
            ["du", "-sh", path],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        if result.returncode == 0:
            return f"{path}: {result.stdout.strip()}"
        else:
            return f"{path}: Error - {result.stderr.strip()}"
    except Exception as e:
        return f"{path}: Exception - {str(e)}"

def parallel_du(paths, max_threads=4):
    """
    多线程执行 du -sh 命令
    """
    results = []
    with ThreadPoolExecutor(max_threads) as executor:
        future_to_path = {executor.submit(get_disk_usage, path): path for path in paths}
        for future in as_completed(future_to_path):
            results.append(future.result())
    return results

if __name__ == "__main__":
    # 获取当前目录下的子目录
    base_dir = "."
    subdirs = [os.path.join(base_dir, d) for d in os.listdir(base_dir) if os.path.isdir(os.path.join(base_dir, d))]
    
    # 运行多线程 du -sh
    output = parallel_du(subdirs, max_threads=4)
    for line in output:
        print(line)
```

---

### **代码解析**

1. **`get_disk_usage(path)`**:
   - 使用 `subprocess.run` 调用 `du -sh`，获取目录的磁盘使用量。
   - 捕获标准输出和错误输出，并返回结果。

2. **`parallel_du(paths, max_threads=4)`**:
   - 使用 `ThreadPoolExecutor` 创建一个线程池。
   - 为每个路径提交 `get_disk_usage` 任务，记录任务和路径的映射。
   - 使用 `as_completed` 收集任务结果，确保线程池能够并发运行。

3. **主程序部分**:
   - 获取当前目录下的所有子目录。
   - 调用 `parallel_du` 函数并打印每个路径的磁盘占用信息。

---

### **运行效果**

假设当前目录下有以下子目录：
```bash
dir1
dir2
dir3
```

运行程序后输出示例：
```bash
./dir1: 20M
./dir2: 15M
./dir3: 10M
```

---

### **注意事项**
1. **并发数控制**：
   - 默认线程池最大线程数为 4，可根据系统性能调整 `max_threads`。

2. **跨平台兼容性**：
   - `du` 是 Linux/Unix 系统命令，需确保系统支持。
   - 如果需要在 Windows 上运行，可以改用 Python 的 `os` 或其他库来计算磁盘占用。

3. **I/O 密集型任务**：
   - 磁盘操作属于 I/O 密集型任务，使用线程比进程更合适。

4. **目录权限**：
   - 运行程序的用户需要对目标目录有读取权限，否则会产生错误输出。

---

如果目录结构复杂且需要递归计算占用，可进一步扩展函数逻辑，将递归功能集成进来。

##  return f"{path}: {result.stdout.strip()}"
这段代码的作用是返回一个格式化的字符串，显示指定路径 (`path`) 的磁盘占用信息，该信息来自 `du -sh` 命令的标准输出结果。

---

### **详细解析**

#### **代码部分**
```python
return f"{path}: {result.stdout.strip()}"
```

#### **功能说明**
1. **`f"{path}: ..."`**
   - 使用 Python 的 **f-string** 格式化字符串。
   - 将路径 `path` 和磁盘占用结果连接在一起，用冒号 `:` 分隔。

2. **`result.stdout.strip()`**
   - `result.stdout`：获取 `subprocess.run` 执行命令后的标准输出。
   - `.strip()`：移除字符串两端的空白字符（包括换行符和空格），确保输出干净整洁。

---

### **执行场景**

假设执行的命令是 `du -sh ./example`，且结果输出如下：
```bash
12M    ./example
```

#### **变量值**
- `path`: `"./example"`
- `result.stdout`: `"12M\t./example\n"`

#### **经过处理**
- `result.stdout.strip()`: `"12M\t./example"`

#### **返回值**
最终返回字符串：
```python
"./example: 12M\t./example"
```

---

### **改进建议**

如果你只需要显示目录的磁盘占用信息，而不包含路径多余部分，可以对结果进行进一步分割和格式化：

#### **改进代码**
```python
return f"{path}: {result.stdout.split()[0]}"
```

#### **结果解析**
- `result.stdout.split()`：将标准输出按空白字符分割，返回列表。
- `split()[0]`：提取磁盘占用部分（例如 `"12M"`）。
- 最终返回值：
  ```python
  "./example: 12M"
  ```

这种方式可以让输出更简洁清晰。

## python 通过命令行输入字符串
在 Python 中，可以通过标准库模块 `sys` 或 `argparse` 处理从命令行输入的字符串。根据需求，可以使用以下几种方法：

---

### **方法一：`input()` 获取用户输入**
适用于交互式输入。

#### **代码示例**
```python
# 从命令行获取用户输入字符串
user_input = input("请输入字符串：")
print(f"你输入的是：{user_input}")
```

#### **运行示例**
```bash
$ python script.py
请输入字符串：Hello, World!
你输入的是：Hello, World!
```

---

### **方法二：通过命令行参数传入字符串**
使用 `sys.argv` 直接读取命令行参数。

#### **代码示例**
```python
import sys

# 检查是否传入了足够的参数
if len(sys.argv) < 2:
    print("用法: python script.py <你的字符串>")
    sys.exit(1)

# 获取命令行中的字符串参数
user_input = sys.argv[1]
print(f"你输入的是：{user_input}")
```

#### **运行示例**
```bash
$ python script.py "Hello, World!"
你输入的是：Hello, World!
```

---

### **方法三：使用 `argparse` 处理命令行输入**
`argparse` 是更强大的方式，可以处理多个参数和选项。

#### **代码示例**
```python
import argparse

# 创建解析器
parser = argparse.ArgumentParser(description="命令行字符串输入示例")
parser.add_argument("input_string", type=str, help="请输入一个字符串")

# 解析参数
args = parser.parse_args()
print(f"你输入的是：{args.input_string}")
```

#### **运行示例**
```bash
$ python script.py "Hello, World!"
你输入的是：Hello, World!
```

---

### **方法四：读取标准输入（管道）**
如果希望通过管道传递字符串，可以使用 `sys.stdin`。

#### **代码示例**
```python
import sys

# 从标准输入读取数据
user_input = sys.stdin.read().strip()
print(f"你输入的是：{user_input}")
```

#### **运行示例**
```bash
$ echo "Hello, World!" | python script.py
你输入的是：Hello, World!
```

---

### **总结**
- 使用 `input()`：适用于交互式输入。
- 使用 `sys.argv` 或 `argparse`：适用于通过命令行传递参数。
- 使用 `sys.stdin`：适用于管道操作或文件重定向。

根据具体场景选择适合的方法！
