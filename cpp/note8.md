### std::make_shared
`std::make_shared` 是 C++ 中用于创建 `std::shared_ptr` 对象的函数模板。它提供了一种更安全、更高效的方式来创建 `std::shared_ptr`，并可以避免 `new` 操作符的直接使用。

`std::make_shared` 的用法如下：

```cpp
#include <memory>

int main() {
    // 创建一个 shared_ptr，指向一个 int 对象
    std::shared_ptr<int> ptr = std::make_shared<int>(42);

    // 创建一个 shared_ptr，指向一个自定义类型的对象
    struct MyType {
        int data;
        MyType(int value) : data(value) {}
    };

    std::shared_ptr<MyType> customPtr = std::make_shared<MyType>(123);

    // ...
    
    return 0;
}
```

使用 `std::make_shared` 的优势在于它在内存管理方面更高效。它在一次动态分配中分配控制块（用于跟踪引用计数等信息）和对象内存，而直接使用 `new` 则需要两次分配，一次给控制块，一次给对象。

此外，`std::make_shared` 的方式更安全，因为它可以防止一些资源泄漏问题。当使用 `std::make_shared` 时，对象和控制块是一起构造的，这样在构造过程中发生异常时，不会导致内存泄漏。

总体而言，推荐在需要创建 `std::shared_ptr` 对象时优先使用 `std::make_shared`，除非有特殊需求需要直接使用 `new`。


### name.c_str()
`name.c_str()` 是用于获取 C 风格字符串（以 null 结尾的字符数组）的成员函数，通常用于将 C++ 的 `std::string` 转换为 C 风格字符串。

在 C++ 中，`std::string` 类提供了一个成员函数 `c_str()`，该函数返回一个指向以 null 结尾的字符数组的指针，即 C 风格字符串。

```cpp
#include <iostream>
#include <string>

int main() {
    std::string name = "Hello, World!";

    // 获取 C 风格字符串
    const char* cString = name.c_str();

    // 输出 C 风格字符串
    std::cout << "C-style String: " << cString << std::endl;

    return 0;
}
```

在这个例子中，`name.c_str()` 返回一个指向字符串 "Hello, World!" 的字符数组的指针，然后可以将该指针传递给期望 C 风格字符串的函数，或者以其他方式使用。

需要注意的是，`c_str()` 返回的指针指向的内存是 `std::string` 内部分配的，如果 `std::string` 被修改或者销毁，这个指针将变得无效。因此，在使用 `c_str()` 的指针时，需要确保在其有效期内使用，以避免悬垂指针的问题。