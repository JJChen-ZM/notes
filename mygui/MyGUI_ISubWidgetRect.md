### MyGUI_ISubWidgetRect.h

这段代码定义了一个名为 `ISubWidgetRect` 的 C++ 类，看起来同样是某个图形用户界面（GUI）框架的一部分。以下是对该类的分析：

1. **继承：**
   ```cpp
   class MYGUI_EXPORT ISubWidgetRect : public ISubWidget
   ```
   该类公开继承了另一个类 `ISubWidget`。这表示 `ISubWidgetRect` 继承了 `ISubWidget` 的属性和行为，并且可能对其进行了扩展或修改。

2. **RTTI（运行时类型信息）：**
   ```cpp
   MYGUI_RTTI_DERIVED( ISubWidgetRect )
   ```
   类可能使用这个宏设置运行时类型信息，以允许在运行时检查对象的类型。

3. **公共成员函数：**
   该类定义了两个虚成员函数，其中：
   - `virtual void _setUVSet(const FloatRect& _rect) { }`：用于设置纹理坐标（UV坐标）。
   - `virtual void _setColour(const Colour& _value) { }`：用于设置颜色。

   这些函数看起来是为了配置矩形子部件的纹理坐标和颜色。这里的下划线可能表示这些函数是供内部使用的，而不是直接由外部用户调用。

4. **数据成员：**
   类本身似乎没有明显的数据成员（成员变量）。这可能意味着该类主要用于提供接口或者作为其他类的基类。

5. **宏 `MYGUI_EXPORT`：**
   这个宏可能用于控制导出符号的行为，通常在库的开发中用于处理导出和导入库的符号。

总体来说，`ISubWidgetRect` 类似乎是一个用于处理矩形子部件的接口或基类。它提供了设置纹理坐标和颜色的虚拟函数，可能用于在 GUI 中绘制矩形元素。实际的实现可能出现在该类的派生类中，或者在框架的其他部分。

### 没有 MyGUI_ISubWidgetRect.cpp