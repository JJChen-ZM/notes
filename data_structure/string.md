## C++ 去掉字符串无效空格
去掉字符串中的无效空格可以分为以下几种情形处理：

1. 删除字符串两端的空格。
2. 将字符串中连续的多个空格压缩为单个空格。
3. 删除字符串中的所有空格。

下面是实现这些功能的代码：

---

### 1. **删除两端的空格**
```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string trim(const string& str) {
    size_t start = str.find_first_not_of(' ');
    size_t end = str.find_last_not_of(' ');
    if (start == string::npos) return ""; // 全是空格
    return str.substr(start, end - start + 1);
}

int main() {
    string str = "   Hello, World!   ";
    cout << "[" << trim(str) << "]" << endl;
    return 0;
}
```

输出：
```
[Hello, World!]
```

---

### 2. **将连续的多个空格压缩为单个空格**
```cpp
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

string reduceSpaces(const string& str) {
    istringstream iss(str);
    string result, word;
    while (iss >> word) {
        if (!result.empty()) result += " ";
        result += word;
    }
    return result;
}

int main() {
    string str = "   This   is    a   test  string.   ";
    cout << "[" << reduceSpaces(str) << "]" << endl;
    return 0;
}
```

输出：
```
[This is a test string.]
```

---

### 3. **删除所有空格**
```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string removeAllSpaces(const string& str) {
    string result = str;
    result.erase(remove(result.begin(), result.end(), ' '), result.end());
    return result;
}

int main() {
    string str = "   Hello,   World!   ";
    cout << "[" << removeAllSpaces(str) << "]" << endl;
    return 0;
}
```

输出：
```
[Hello,World!]
```

---

### **综合实现**
将所有上述功能封装在一个程序中，可以选择需要的操作：

```cpp
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

string trim(const string& str) {
    size_t start = str.find_first_not_of(' ');
    size_t end = str.find_last_not_of(' ');
    if (start == string::npos) return ""; // 全是空格
    return str.substr(start, end - start + 1);
}

string reduceSpaces(const string& str) {
    istringstream iss(str);
    string result, word;
    while (iss >> word) {
        if (!result.empty()) result += " ";
        result += word;
    }
    return result;
}

string removeAllSpaces(const string& str) {
    string result = str;
    result.erase(remove(result.begin(), result.end(), ' '), result.end());
    return result;
}

int main() {
    string str = "   Hello,   World!   ";

    cout << "Original: [" << str << "]" << endl;
    cout << "Trimmed: [" << trim(str) << "]" << endl;
    cout << "Reduced spaces: [" << reduceSpaces(str) << "]" << endl;
    cout << "All spaces removed: [" << removeAllSpaces(str) << "]" << endl;

    return 0;
}
```

输出：
```
Original: [   Hello,   World!   ]
Trimmed: [Hello,   World!]
Reduced spaces: [Hello, World!]
All spaces removed: [Hello,World!]
```

---

### **总结**
- **`trim`**：删除字符串两端的空格。
- **`reduceSpaces`**：将连续的多个空格压缩为一个。
- **`removeAllSpaces`**：删除字符串中所有空格。








## istringstream 
`std::istringstream` 是 C++ 标准库中的一个输入字符串流类，位于头文件 `<sstream>` 中。它用于从字符串中解析数据，类似于如何从文件或标准输入流中读取数据。

---

### **基本功能**
`std::istringstream` 可以：
1. **从字符串中提取数据**，像处理文件或控制台输入一样。
2. **分割字符串**，特别是在处理空格分隔的数据时很方便。

---

### **基本用法**
#### **1. 创建并初始化 `istringstream`**
```cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    string data = "123 45.67 Hello";
    istringstream iss(data);

    int i;
    double d;
    string s;

    // 从字符串流中依次提取数据
    iss >> i >> d >> s;

    cout << "Integer: " << i << endl;
    cout << "Double: " << d << endl;
    cout << "String: " << s << endl;

    return 0;
}
```

**输出：**
```
Integer: 123
Double: 45.67
String: Hello
```

---

#### **2. 循环读取字符串中的单词**
```cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    string sentence = "This is a test string";
    istringstream iss(sentence);
    string word;

    while (iss >> word) {
        cout << word << endl;
    }

    return 0;
}
```

**输出：**
```
This
is
a
test
string
```

---

#### **3. 处理带分隔符的字符串**
如果字符串中使用逗号或其他字符作为分隔符，可以使用 `std::getline`。

```cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    string data = "apple,banana,orange,grape";
    istringstream iss(data);
    string fruit;

    // 使用逗号作为分隔符读取
    while (getline(iss, fruit, ',')) {
        cout << fruit << endl;
    }

    return 0;
}
```

**输出：**
```
apple
banana
orange
grape
```

---

#### **4. 转换字符串到数字**
`std::istringstream` 常用于将字符串转换为数字。

```cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    string numberStr = "42";
    istringstream iss(numberStr);

    int number;
    iss >> number;

    cout << "Converted number: " << number << endl;

    return 0;
}
```

**输出：**
```
Converted number: 42
```

---

### **特点与注意事项**
1. **自动跳过空格**：
   - `>>` 操作符会自动跳过空格和换行符。
   - 如果要保留空格，使用 `getline`。

2. **状态检查**：
   - 在读取数据后，可以检查流的状态，比如 `iss.fail()` 来判断是否读取失败。

3. **灵活的初始化**：
   - 可以直接用字符串初始化 `istringstream`，也可以使用 `iss.str(newString)` 重新设置流内容。

4. **效率高**：
   - `std::istringstream` 适合处理较小的字符串，对于大规模字符串操作可能需要更高效的库或方法。

---

### **总结**
`std::istringstream` 是处理字符串数据的强大工具，适合用来解析和处理以空格或特定分隔符分隔的输入数据，特别是在文件读取、命令行解析、以及字符串格式化转换中。




## 实现 strStr() 函数。

给定一个 haystack 字符串和一个 needle 字符串，在 haystack 字符串中找出 needle 字符串出现的第一个位置 (从0开始)。如果不存在，则返回  -1。

实现 `strStr()` 函数的目标是在一个字符串中找到另一个字符串的第一个出现位置。可以用简单的滑动窗口实现，也可以使用更高效的算法如 KMP（Knuth-Morris-Pratt）。以下是简单实现和 KMP 实现两种方法的代码。

---

### **方法 1：简单实现（暴力匹配）**
```cpp
#include <iostream>
#include <string>
using namespace std;

int strStr(const string &haystack, const string &needle) {
    if (needle.empty()) return 0; // 如果 needle 为空字符串，返回 0
    int n = haystack.size(), m = needle.size();

    for (int i = 0; i <= n - m; ++i) {
        // 比较子串是否匹配
        if (haystack.substr(i, m) == needle) {
            return i;
        }
    }
    return -1; // 没有匹配到返回 -1
}

int main() {
    string haystack = "hello";
    string needle = "ll";
    cout << "Result: " << strStr(haystack, needle) << endl; // 输出 2
    return 0;
}
```

---

### **方法 2：优化实现（滑动窗口）**
通过滑动窗口逐字符比较，提高效率。

```cpp
#include <iostream>
#include <string>
using namespace std;

int strStr(const string &haystack, const string &needle) {
    if (needle.empty()) return 0;
    int n = haystack.size(), m = needle.size();

    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && haystack[i + j] == needle[j]) {
            ++j;
        }
        if (j == m) {
            return i; // 找到匹配的子串
        }
    }
    return -1; // 没有匹配到返回 -1
}

int main() {
    string haystack = "hello";
    string needle = "ll";
    cout << "Result: " << strStr(haystack, needle) << endl; // 输出 2
    return 0;
}
```

---

### **方法 3：高效实现（KMP 算法）**
KMP 算法通过预处理 `needle` 构建部分匹配表（next 数组），实现更高效的匹配。

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 构建部分匹配表
vector<int> buildNext(const string &needle) {
    int m = needle.size();
    vector<int> next(m, 0);
    int j = 0; // 前缀长度
    for (int i = 1; i < m; ++i) {
        while (j > 0 && needle[i] != needle[j]) {
            j = next[j - 1];
        }
        if (needle[i] == needle[j]) {
            ++j;
        }
        next[i] = j;
    }
    return next;
}

// KMP 主函数
int strStr(const string &haystack, const string &needle) {
    if (needle.empty()) return 0;
    int n = haystack.size(), m = needle.size();
    vector<int> next = buildNext(needle);

    int j = 0; // needle 的匹配长度
    for (int i = 0; i < n; ++i) {
        while (j > 0 && haystack[i] != needle[j]) {
            j = next[j - 1]; // 回退到上一个部分匹配位置
        }
        if (haystack[i] == needle[j]) {
            ++j;
        }
        if (j == m) {
            return i - m + 1; // 找到匹配，返回起始位置
        }
    }
    return -1; // 没有匹配到
}

int main() {
    string haystack = "hello";
    string needle = "ll";
    cout << "Result: " << strStr(haystack, needle) << endl; // 输出 2
    return 0;
}
```

---

### **方法对比**
| 方法           | 时间复杂度         | 空间复杂度 | 适用场景                        |
|----------------|------------------|----------|--------------------------------|
| 简单实现       | \(O((n-m+1) \cdot m)\) | \(O(1)\)   | 子串较短，算法简单易实现         |
| 滑动窗口       | \(O((n-m+1) \cdot m)\) | \(O(1)\)   | 对比简单实现，直接字符比较效率高 |
| KMP 算法       | \(O(n + m)\)        | \(O(m)\)   | 长字符串、高性能需求            |

---

### **示例输出**
对于输入：
```cpp
string haystack = "hello";
string needle = "ll";
```
输出结果为：
```
Result: 2
```