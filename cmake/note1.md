## cmake add_library
`add_library`是CMake中用于创建库（静态或共享）的命令。下面是一个简单的示例，展示如何使用`add_library`：

```cmake
# 定义一个名为myLibrary的库，包含两个源文件
add_library(myLibrary source1.cpp source2.cpp)

# 将库链接到目标可执行文件
add_executable(myExecutable main.cpp)
target_link_libraries(myExecutable myLibrary)
```

在上述示例中，`add_library`创建了一个名为`myLibrary`的库，它包含两个源文件`source1.cpp`和`source2.cpp`。然后，`add_executable`创建了一个名为`myExecutable`的目标可执行文件，并使用`target_link_libraries`将`myLibrary`库链接到`myExecutable`。

## CMake target_include_directories
`target_include_directories`是CMake中用于为目标（如可执行文件、库等）指定头文件搜索路径的命令。以下是一个简单的示例：

```cmake
# 定义一个名为myLibrary的库，包含两个源文件和头文件目录
add_library(myLibrary source1.cpp source2.cpp)
target_include_directories(myLibrary PUBLIC include)

# 将库链接到目标可执行文件，同时指定头文件目录
add_executable(myExecutable main.cpp)
target_link_libraries(myExecutable myLibrary)
```

在上述示例中，`target_include_directories`用于为`myLibrary`库指定头文件目录，这里是`include`目录。通过将`PUBLIC`关键字添加到`target_include_directories`，可以使这个目录对链接到`myLibrary`的目标（如`myExecutable`）可见。



