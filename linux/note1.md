### ubuntu C++ 编译器
在 Ubuntu 上，你可以使用多个 C++ 编译器来编译 C++ 程序。以下是在 Ubuntu 上常用的几种 C++ 编译器：

1. GCC (GNU Compiler Collection)：GCC 是一个功能强大且广泛使用的编译器套件，它包含了 C、C++、Fortran 等多种编程语言的编译器。在大多数 Ubuntu 系统中，GCC 是默认安装的。你可以使用以下命令来安装 GCC：

   ```
   sudo apt-get update
   sudo apt-get install g++
   ```

   安装完成后，你可以使用 `g++` 命令来编译 C++ 程序。

2. Clang：Clang 是另一个流行的 C++ 编译器，它是 LLVM 项目的一部分。Clang 提供了高质量的诊断信息和更快的编译速度。你可以使用以下命令来安装 Clang：

   ```
   sudo apt-get update
   sudo apt-get install clang
   ```

   安装完成后，你可以使用 `clang++` 命令来编译 C++ 程序。

3. Intel C++ Compiler：如果你使用 Intel 处理器并想要使用优化的特定功能，你还可以考虑使用 Intel C++ 编译器。Intel C++ 编译器提供了针对 Intel 处理器的优化，并且可以提高性能。你可以在 Intel 的官方网站上获取更多关于安装和使用 Intel C++ 编译器的信息。

对于大多数情况，GCC 和 Clang 都是常用的选择，它们都是开源的，并且在 Ubuntu 中易于安装和使用。选择哪个编译器取决于你的需求和喜好。通常情况下，你可以在终端中使用命令来编译你的 C++ 程序，例如：

```
g++ -o my_program my_program.cpp
```

或者

```
clang++ -o my_program my_program.cpp
```

上述命令将会将名为 `my_program.cpp` 的 C++ 源代码编译成一个可执行文件 `my_program`。如果编译成功，你可以通过运行 `./my_program` 命令来执行你的程序。

### .mk 文件
在计算机领域，`.mk` 文件通常指的是 Makefile 文件。Makefile 是一种用于构建和编译软件项目的脚本文件，它使用 Make 工具来自动化构建过程。

Makefile 文件通常包含一系列规则（rules），每个规则定义了如何根据源文件生成目标文件，并指定依赖关系。当你运行 `make` 命令时，Make 工具会读取 Makefile 文件并根据规则执行所需的编译和构建操作。

Makefile 文件中的规则通常具有以下格式：

```
target: dependencies
    command
```

- `target` 是一个目标文件，表示要生成的输出文件。
- `dependencies` 是目标文件所依赖的源文件或其他目标文件。
- `command` 是执行构建过程的命令。

一个简单的示例 Makefile 文件如下：

```makefile
# Makefile 示例

# 定义目标文件和依赖关系
app: main.o utils.o
    g++ main.o utils.o -o app

# 定义生成 main.o 文件的规则
main.o: main.cpp
    g++ -c main.cpp

# 定义生成 utils.o 文件的规则
utils.o: utils.cpp
    g++ -c utils.cpp
```

在上述示例中，Makefile 定义了一个名为 `app` 的目标文件，它依赖于 `main.o` 和 `utils.o`。然后，它定义了两个规则，分别用于生成 `main.o` 和 `utils.o`，并指定了编译的命令。

通过运行 `make` 命令，Make 工具将根据 Makefile 文件中的规则自动编译源文件，并生成目标文件 `app`。这样，你就可以使用 Makefile 来自动构建和编译你的软件项目。

### grep -rn 
`grep -rn` 是一个在 Linux/Unix 系统中常用的命令，用于在文件中递归搜索指定的文本内容，并显示匹配的行号和文件名。

命令的结构如下：

```
grep -rn "要搜索的文本" 目标目录或文件
```

其中：

- `-r` 表示递归搜索，会在目标目录及其子目录下进行搜索。
- `-n` 表示显示匹配的行号。
- "要搜索的文本" 是你想要查找的具体文本内容。
- `目标目录或文件` 是你希望在哪个目录下或哪个文件中进行搜索。

例如，如果你想在当前目录及其子目录下搜索包含 "hello" 这个文本的文件，并显示匹配的行号和文件名，可以使用以下命令：

```
grep -rn "hello" .
```

注意，`.` 表示当前目录，你可以将其替换为其他目录路径或特定文件名，以在不同的位置进行搜索。

使用 `grep -rn` 可以帮助你快速找到包含特定文本的文件，并定位到匹配的行号，是一种非常有用的文本搜索工具。