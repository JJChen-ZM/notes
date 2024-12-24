## g++、make、cmake
`g++`、`make` 和 `cmake` 是软件开发中常用的工具，它们各自的功能和应用范围如下：

---

## **1. g++（GNU C++ 编译器）**
### **功能**
- **g++** 是 GCC（GNU Compiler Collection）的一部分，用于编译和链接 C++ 程序。
- 它将 C++ 源代码文件（`.cpp`、`.h`）转换为可执行文件或库。

### **常用命令**
- **编译单个文件**：
  ```bash
  g++ -o program main.cpp
  ```
  将 `main.cpp` 编译成可执行文件 `program`。

- **编译多个文件**：
  ```bash
  g++ -o program main.cpp utils.cpp
  ```

- **生成目标文件（不链接）**：
  ```bash
  g++ -c main.cpp -o main.o
  ```
  生成目标文件 `main.o`。

- **指定标准**（如 C++11 或 C++17）：
  ```bash
  g++ -std=c++17 -o program main.cpp
  ```

- **启用优化**（如 `-O2`）：
  ```bash
  g++ -O2 -o program main.cpp
  ```

---

## **2. make（构建工具）**
### **功能**
- **make** 是一个构建自动化工具，通过 **Makefile** 来定义如何编译和链接程序。
- 它根据文件的依赖关系，只重新编译那些需要更新的部分，提高了构建效率。

### **常用步骤**
1. **编写 Makefile**：
   示例 Makefile：
   ```makefile
   program: main.o utils.o
       g++ -o program main.o utils.o

   main.o: main.cpp
       g++ -c main.cpp -o main.o

   utils.o: utils.cpp
       g++ -c utils.cpp -o utils.o

   clean:
       rm -f *.o program
   ```

2. **使用 make 构建**：
   ```bash
   make
   ```
   执行默认目标（如 `program`）。
   
3. **清理构建文件**：
   ```bash
   make clean
   ```

---

## **3. cmake（跨平台构建工具）**
### **功能**
- **CMake** 是一个更高级的构建工具，生成适合特定平台的构建系统（如 Makefile、Ninja 文件或 Visual Studio 项目）。
- 它通过 `CMakeLists.txt` 文件定义构建逻辑，并支持多种平台和编译器。

### **常用步骤**
1. **创建 CMakeLists.txt 文件**：
   示例：
   ```cmake
   cmake_minimum_required(VERSION 3.10)

   # 项目信息
   project(MyProject)

   # 添加可执行文件
   add_executable(program main.cpp utils.cpp)
   ```

2. **生成 Makefile**：
   ```bash
   cmake .
   ```

3. **编译项目**：
   ```bash
   make
   ```

4. **构建输出到指定目录**：
   ```bash
   cmake -B build .
   cmake --build build
   ```

---

## **三者之间的区别与联系**

| **工具**    | **作用**                                           | **特点**                                                                 |
|-------------|---------------------------------------------------|--------------------------------------------------------------------------|
| **g++**     | 编译器，用于编译和链接代码。                         | 直接调用，适合简单项目；需要手动处理依赖关系。                                   |
| **make**    | 构建自动化工具，通过 Makefile 管理项目的编译过程。     | 自动管理依赖文件，适合中小型项目；Makefile 灵活但语法有限。                      |
| **cmake**   | 构建系统生成工具，用于生成 Makefile 或其他构建系统文件。 | 跨平台支持，适合大型项目；配置更复杂，但更现代化，支持多种 IDE 和工具链。          |

---

### **如何选择**
- **简单项目**：直接使用 `g++`。
- **中小型项目**：用 `make` 管理构建。
- **跨平台或复杂项目**：使用 `cmake`。




## cmake结束以后还需要make
是的，**CMake** 和 **Make** 通常是结合使用的，具体流程如下：

---

### **CMake 与 Make 的关系**
- **CMake** 是一个生成工具，生成适合特定构建系统的文件，例如 **Makefile**。
- **Make** 是一个执行工具，基于 CMake 生成的 **Makefile** 实际完成编译、链接等任务。

换句话说：
1. **CMake** 的主要任务是生成构建配置文件（如 Makefile）。
2. **Make** 的主要任务是根据这些配置文件执行编译和构建。

---

### **CMake 构建流程**
以下是完整的流程：

1. **创建 CMakeLists.txt 文件**  
   示例：
   ```cmake
   cmake_minimum_required(VERSION 3.10)
   project(MyProject)

   # 指定可执行文件
   add_executable(program main.cpp utils.cpp)
   ```

2. **运行 CMake**  
   在项目根目录中执行：
   ```bash
   cmake .
   ```
   或者，生成文件到独立的构建目录（推荐）：
   ```bash
   cmake -B build .
   ```

   这会生成适合当前平台的构建配置文件，例如 Makefile。

3. **运行 Make**  
   如果是在当前目录，直接运行：
   ```bash
   make
   ```
   如果生成文件在 `build` 目录，则运行：
   ```bash
   make -C build
   ```
   或者：
   ```bash
   cd build
   make
   ```

4. **生成最终的可执行文件或库**  
   如果一切成功，Make 会根据配置生成目标文件，如 `program`。

---

### **为什么需要先用 CMake 再用 Make？**
- **CMake**：负责处理跨平台支持、依赖管理等复杂逻辑，但不负责实际编译。
- **Make**：高效执行 CMake 生成的构建规则，进行实际的代码编译和链接。

---

### **简化流程**
如果不想手动运行两步，可以使用：
```bash
cmake -B build .
cmake --build build
```
这两条命令相当于：
1. 运行 `cmake` 生成文件。
2. 自动调用 `make` 或其他生成的构建系统（如 Ninja）完成构建。