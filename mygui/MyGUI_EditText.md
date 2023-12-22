### MyGUI_EditText.h

这是 `EditText` 类的声明，以下是该类的主要功能和成员变量的解释：

1. **构造函数：**
   ```cpp
   EditText::EditText()
   ```
   构造函数初始化 `EditText` 类的实例。

2. **setVisible 函数：**
   ```cpp
   void EditText::setVisible(bool _visible) override;
   ```
   该函数用于设置文本编辑框的可见性。

3. **updateRawData 函数：**
   ```cpp
   void EditText::updateRawData() const;
   ```
   该函数用于在常量方法中使用，但实际上可能会更新对象的原始数据。

4. **doRender 函数：**
   ```cpp
   void EditText::doRender() override;
   ```
   该函数用于执行文本编辑框的渲染。

5. **setCaption 函数：**
   ```cpp
   void EditText::setCaption(const UString& _value) override;
   ```
   该函数用于设置文本编辑框的标题。

6. **getCaption 函数：**
   ```cpp
   const UString& EditText::getCaption() const override;
   ```
   该函数用于获取文本编辑框的标题。

7. **setTextColour 函数：**
   ```cpp
   void EditText::setTextColour(const Colour& _value) override;
   ```
   该函数用于设置文本颜色。

8. **getTextColour 函数：**
   ```cpp
   const Colour& EditText::getTextColour() const override;
   ```
   该函数用于获取文本颜色。

9. **setAlpha 函数：**
   ```cpp
   void EditText::setAlpha(float _value) override;
   ```
   该函数用于设置文本透明度。

10. **setFontName 函数：**
    ```cpp
    void EditText::setFontName(std::string_view _value) override;
    ```
    该函数用于设置文本编辑框的字体名称。

11. **getFontName 函数：**
    ```cpp
    std::string_view EditText::getFontName() const override;
    ```
    该函数用于获取文本编辑框的字体名称。

12. **setFontHeight 函数：**
    ```cpp
    void EditText::setFontHeight(int _value) override;
    ```
    该函数用于设置文本编辑框的字体高度。

13. **getFontHeight 函数：**
    ```cpp
    int EditText::getFontHeight() const override;
    ```
    该函数用于获取文本编辑框的字体高度。

14. **createDrawItem 函数：**
    ```cpp
    void EditText::createDrawItem(ITexture* _texture, ILayerNode* _node) override;
    ```
    该函数用于创建文本编辑框的绘制项。

15. **destroyDrawItem 函数：**
    ```cpp
    void EditText::destroyDrawItem() override;
    ```
    该函数用于销毁文本编辑框的绘制项。

16. **setTextAlign 函数：**
    ```cpp
    void EditText::setTextAlign(Align _value) override;
    ```
    该函数用于设置文本对齐方式。

17. **getTextAlign 函数：**
    ```cpp
    Align EditText::getTextAlign() const override;
    ```
    该函数用于获取文本对齐方式。

18. **getTextSelectionStart 函数：**
    ```cpp
    size_t EditText::getTextSelectionStart() const override;
    ```
    该函数用于获取文本选择的起始位置。

19. **getTextSelectionEnd 函数：**
    ```cpp
    size_t EditText::getTextSelectionEnd() const override;
    ```
    该函数用于获取文本选择的结束位置。

20. **setTextSelection 函数：**
    ```cpp
    void EditText::setTextSelection(size_t _start, size_t _end) override;
    ```
    该函数用于设置文本选择的范围。

21. **getSelectBackground 函数：**
    ```cpp
    bool EditText::getSelectBackground() const override;
    ```
    该函数用于获取文本选择区域的背景是否可见。

22. **setSelectBackground 函数：**
    ```cpp
    void EditText::setSelectBackground(bool _normal) override;
    ```
    该函数用于设置文本选择区域的背景是否可见。

23. **isVisibleCursor 函数：**
    ```cpp
    bool EditText::isVisibleCursor() const override;
    ```
    该函数用于判断文本编辑框的光标是否可见。

24. **setVisibleCursor 函数：**
    ```cpp
    void EditText::setVisibleCursor(bool _value) override;
    ```
    该函数用于设置文本编辑框的光标可见性。

25. **getInvertSelected 函数：**
    ```cpp
    bool EditText::getInvertSelected() const override;
    ```
    该函数用于获取是否反转选择的文本颜色。

26. **setInvertSelected 函数：**
    ```cpp
    void EditText::setInvertSelected(bool _value) override;
    ```
    该函数用于设置是否反转选择的文本颜色。

27. **getCursorPosition 函数：**
    ```cpp
    size_t EditText::getCursorPosition() const override;
    ```
    该函数用于获取光标的位置。

28. **setCursorPosition 函数：**
    ```cpp
    void EditText::setCursorPosition(size_t _index) override;
    ```
    该函数用于设置光标的位置。

29. **getTextSize 函数：**
    ```cpp
    IntSize EditText::getTextSize() const override;
    ```
    该函数用于获取文本的大小。

30. **setViewOffset 函数：**
    ```cpp
    void EditText::setViewOffset(const IntPoint& _point) override;
    ```
    该函数用于设置文本视图偏移。



### MyGUI_EditText.cpp

这段 C++ 代码定义了 `EditText` 类的实现，它是 MyGUI 库的一部分。MyGUI 是一个用于 C++ 的开源 GUI（图形用户界面）库。让我们分解代码的重要方面：

1. **类定义：**
    - `EditText` 类派生自 `ISubWidgetText`，是 MyGUI 库的一部分。
    - 它包含各种与文本渲染和操作相关的方法和成员变量。

2. **构造函数：**
    - 构造函数使用常量值（`SIMPLETEXT_COUNT_VERTEX`）初始化 `mCountVertex` 成员，并使用函数调用 `RenderManager::getInstance().getVertexFormat()` 设置 `mVertexFormat` 成员。

3. **可见性和渲染：**
    - `setVisible` 方法处理 `EditText` 对象的可见性，并在必要时触发更新。
    - `_correctView` 和 `_updateView` 等方法涉及处理渲染和视图更新。

4. **文本处理：**
    - `setCaption`、`getCaption`、`setTextColour`、`getAlpha` 等方法与管理和检索文本属性有关。
    - `checkVertexSize` 方法根据文本的大小重新分配顶点缓冲区。

5. **字体和渲染项管理：**
    - `setFontName`、`getFontName`、`setFontHeight`、`getFontHeight`、`createDrawItem` 和 `destroyDrawItem` 等方法涉及字体管理和处理渲染项。

6. **选择和光标：**
    - `getTextSelectionStart`、`getTextSelectionEnd`、`setTextSelection`、`getSelectBackground`、`setSelectBackground`、`isVisibleCursor`、`setCursorPosition` 等方法处理文本选择和光标定位。

7. **对齐和视图偏移：**
    - `setTextAlign`、`getTextAlign`、`setViewOffset` 和 `getViewOffset` 等方法与文本对齐和处理视图偏移有关。

8. **渲染方法：**
    - `drawQuad` 和 `drawGlyph` 等方法涉及渲染文本和相关元素。

9. **状态数据和更新：**
    - `setStateData` 方法处理状态信息，而 `_updateView` 和 `updateRawData` 等方法涉及更新视图和原始文本数据。

10. **颜色和阴影处理：**
    - `_setTextColour`、`getTextColour`、`setAlpha`、`setShadow`、`getShadow`、`setShadowColour` 和 `getShadowColour` 等方法处理颜色和阴影属性。

11. **反转选择：**
    - `setInvertSelected` 和 `getInvertSelected` 等方法处理反转选择文本颜色。

这个类似乎管理了与在图形用户界面中渲染和与文本交互相关的许多方面，提供了处理各种文本元素属性和行为的方法。