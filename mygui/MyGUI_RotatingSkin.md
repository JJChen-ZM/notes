### MyGUI_RotatingSkin.h

这是一个名为 `RotatingSkin` 的C++类，看起来是某个图形用户界面（GUI）框架的一部分。以下是对该类的主要特征和功能的分析：

1. **继承：**
   ```cpp
   class MYGUI_EXPORT RotatingSkin : public ISubWidgetRect
   ```
   该类继承自 `ISubWidgetRect`，表示 `RotatingSkin` 类继承了矩形子部件的属性和行为。

2. **RTTI（运行时类型信息）：**
   ```cpp
   MYGUI_RTTI_DERIVED(RotatingSkin)
   ```
   该宏可能用于设置运行时类型信息，允许在运行时检查对象的类型。

3. **构造函数：**
   ```cpp
   RotatingSkin();
   ```
   一个默认构造函数，可能用于创建 `RotatingSkin` 的实例。

4. **公共成员函数：**
   - `setAngle(float _angle)`: 设置旋转角度（弧度）。
   - `getAngle() const`: 获取旋转角度。
   - `setCenter(const IntPoint& _center)`: 设置旋转中心点。
   - `getCenter(bool _local = true) const`: 获取旋转中心点。
   - `setAlpha(float _alpha) override`: 设置透明度的重写函数。
   - `setVisible(bool _visible) override`: 设置可见性的重写函数。
   - `setStateData(IStateInfo* _data) override`: 设置状态数据的重写函数。
   - `createDrawItem(ITexture* _texture, ILayerNode* _node) override`: 创建绘制项的重写函数。
   - `destroyDrawItem() override`: 销毁绘制项的重写函数。
   - `doRender() override`: 执行渲染的重写函数。
   - `_updateView() override`: 更新视图的内部函数。
   - `_correctView() override`: 修正视图的内部函数。
   - `_setAlign(const IntSize& _oldsize) override`: 设置对齐的内部函数。
   - `_setUVSet(const FloatRect& _rect) override`: 设置纹理坐标的内部函数。
   - `_setColour(const Colour& _value) override`: 设置颜色的内部函数。

5. **保护成员函数：**
   - `_rebuildGeometry()`: 重建几何信息的内部函数。

6. **私有成员变量：**
   - `mGeometryOutdated`: 一个标志，指示几何信息是否过时。
   - `mAngle`: 存储旋转角度的变量。
   - `mCenterPos`: 存储旋转中心点的变量。
   - `mResultVerticiesPos` 和 `mResultVerticiesUV`: 存储几何信息的数组。
   - `mEmptyView`: 一个标志，指示视图是否为空。
   - `mVertexFormat` 和 `mCurrentColour`: 存储颜色信息的变量。
   - `mCurrentTexture` 和 `mCurrentCoord`: 存储当前纹理和坐标信息的变量。
   - `mNode` 和 `mRenderItem`: 存储渲染项相关信息的变量。

这个类似乎用于表示可以旋转的矩形子部件，提供了一系列用于设置、获取和渲染的函数。`_rebuildGeometry` 函数可能用于重新计算几何信息。



### MyGUI_RotatingSkin.cpp

这是 `RotatingSkin` 类的实现的一部分，以下是对一些主要函数的解析：

1. **构造函数：**
   ```cpp
   RotatingSkin::RotatingSkin()
   {
       mVertexFormat = RenderManager::getInstance().getVertexFormat();
   }
   ```
   构造函数初始化了 `mVertexFormat`，该成员变量存储了图形渲染的顶点格式。

2. **setAngle 函数：**
   ```cpp
   void RotatingSkin::setAngle(float _angle)
   {
       mAngle = _angle;
       mGeometryOutdated = true;

       if (nullptr != mNode)
           mNode->outOfDate(mRenderItem);
   }
   ```
   该函数用于设置旋转角度，并标记几何信息为过时，以便在下一次渲染时重新构建。

3. **setCenter 函数：**
   ```cpp
   void RotatingSkin::setCenter(const IntPoint& _center)
   {
       mCenterPos = _center;
       mGeometryOutdated = true;

       if (nullptr != mNode)
           mNode->outOfDate(mRenderItem);
   }
   ```
   该函数用于设置旋转中心点，并标记几何信息为过时，以便在下一次渲染时重新构建。

4. **getCenter 函数：**
   ```cpp
   IntPoint RotatingSkin::getCenter(bool _local) const
   {
       return mCenterPos + (_local ? IntPoint() : mCroppedParent->getAbsolutePosition());
   }
   ```
   该函数用于获取旋转中心点的位置，如果 `_local` 为 `true`，则返回本地坐标，否则返回绝对坐标。

5. **setVisible 函数：**
   ```cpp
   void RotatingSkin::setVisible(bool _visible)
   {
       if (mVisible == _visible)
           return;

       mVisible = _visible;
       mGeometryOutdated = true;

       if (nullptr != mNode)
           mNode->outOfDate(mRenderItem);
   }
   ```
   该函数用于设置控件的可见性，并标记几何信息为过时，以便在下一次渲染时重新构建。

6. **setAlpha 函数：**
   ```cpp
   void RotatingSkin::setAlpha(float _alpha)
   {
       uint32 alpha = ((uint8)(_alpha * 255) << 24);
       mCurrentColour = (mCurrentColour & 0x00FFFFFF) | (alpha & 0xFF000000);

       if (nullptr != mNode)
           mNode->outOfDate(mRenderItem);
   }
   ```
   该函数用于设置控件的透明度，并将颜色信息存储在 `mCurrentColour` 中。

7. **_correctView 函数：**
   ```cpp
   void RotatingSkin::_correctView()
   {
       mGeometryOutdated = true;

       if (nullptr != mNode)
           mNode->outOfDate(mRenderItem);
   }
   ```
   该函数用于修正视图，并标记几何信息为过时，以便在下一次渲染时重新构建。

8. **_setAlign 函数：**
   ```cpp
   void RotatingSkin::_setAlign(const IntSize& _oldsize)
   {
       // 实现对齐的逻辑，具体过程略
   }
   ```
   该函数用于设置对齐，实现对齐的逻辑。

9. **_updateView 函数：**
   ```cpp
   void RotatingSkin::_updateView()
   {
       mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

       mGeometryOutdated = true;

       if (nullptr != mNode)
           mNode->outOfDate(mRenderItem);
   }
   ```
   该函数用于更新视图，并标记几何信息为过时，以便在下一次渲染时重新构建。

10. **createDrawItem 函数：**
   ```cpp
   void RotatingSkin::createDrawItem(ITexture* _texture, ILayerNode* _node)
   {
       MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

       mNode = _node;
       mRenderItem = mNode->addToRenderItem(_texture, true, false);
       mRenderItem->addDrawItem(this, (GEOMETRY_VERTICIES_TOTAL_COUNT - 2) * 3);
   }
   ```
   该函数用于创建绘制项，并将其添加到渲染项中。

11. **destroyDrawItem 函数：**
   ```cpp
   void RotatingSkin::destroyDrawItem()
   {
       MYGUI_ASSERT(mRenderItem, "mRenderItem must be not nullptr");

       mNode = nullptr;
       mRenderItem->removeDrawItem(this);
       mRenderItem = nullptr;
   }
   ```
   该函数用于销毁绘制项。

12. **doRender 函数：**
   ```cpp
   void RotatingSkin::doRender()
   {
       // 实现渲染的逻辑，具体过程略
   }
   ```
   该函数用于执行渲染。

13. **_setColour 函数：**
   ```cpp
   void RotatingSkin::_setColour(const Colour& _value)
   {
       uint32 colour = texture_utility::toNativeColour(_value, mVertexFormat);
       mCurrentColour