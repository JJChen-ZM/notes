
### mTextBox->getLayerNode();
当我们解析表达式 `mTextBox->getLayerNode();` 时：

1. **`mTextBox`**：这是指向 `MyGUI::TextBox` 类实例的指针。它代表你创建的 TextBox 控件。

2. **`->`**：这是与指针一起使用的成员访问运算符。它允许你访问指针所指向的对象的成员（变量或函数）。

3. **`getLayerNode()`**：这是由 `MyGUI::Widget` 类（以及其派生类如 `MyGUI::TextBox`）提供的一个方法。`getLayerNode` 方法用于检索与部件关联的 `MyGUI::LayerNode`。`LayerNode` 负责管理部件的可视层次和渲染属性。

因此，当你调用 `mTextBox->getLayerNode();` 时，你正在使用 `MyGUI::TextBox` 类的 `getLayerNode` 方法来检索与该 TextBox 控件关联的 `LayerNode`。然后，将此 `LayerNode` 存储在 `mLayerNode` 变量中以供以后使用，例如设置旋转角度。

总结一下，`mTextBox->getLayerNode();` 获取与 TextBox 控件 `mTextBox` 关联的渲染层节点，允许你操作其渲染属性。


### MYGUI::LayerNode
在 MyGUI 中，`MyGUI::LayerNode` 是一个用于处理图形层次结构的类。这个类通常用于管理 GUI 元素的可视层次、渲染属性和变换效果。以下是一些关于 `MyGUI::LayerNode` 类的基本信息：

1. **层次结构：** `MyGUI::LayerNode` 类表示 GUI 控件的层次结构中的一个节点。不同的层次结构节点可以有不同的渲染属性，例如透明度和旋转。

2. **渲染属性：** `LayerNode` 允许你设置和获取与渲染相关的属性，如透明度、颜色等。这些属性影响控件的外观。

3. **变换效果：** 你可以使用 `LayerNode` 实现一些基本的变换效果，如平移、缩放和旋转。这对于创建动画效果或调整控件的位置非常有用。

以下是一个简单的示例，演示如何在 MyGUI 中使用 `LayerNode`：

```cpp
#include <MyGUI.h>

int main()
{
    // 初始化 MyGUI
    MyGUI::Gui* gui = new MyGUI::Gui();
    gui->initialise();

    // 创建一个窗口
    MyGUI::Window* myWindow = gui->createWidget<MyGUI::Window>("Window", 100, 100, 200, 150, MyGUI::Align::Default, "Main");
    myWindow->setCaption("My Window");

    // 获取窗口的 LayerNode
    MyGUI::LayerNode* layerNode = myWindow->getLayerNode();

    // 设置透明度
    layerNode->setAlpha(0.8);

    // 设置旋转角度
    layerNode->setRotating(MyGUI::Degree(45));

    // 主循环或其他应用程序逻辑

    // 清理 MyGUI
    gui->shutdown();
    delete gui;

    return 0;
}
```

在这个例子中，`myWindow->getLayerNode()` 获取了窗口的 `LayerNode`，然后通过 `setAlpha` 和 `setRotating` 方法设置了透明度和旋转角度。这只是一个简单的示例，你可以根据需要进行更复杂的设置和操作。