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