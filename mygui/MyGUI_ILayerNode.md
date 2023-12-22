### MyGUI_ILayerNode.h
这是一个 C++ 类的定义，名为 `ILayerNode`，它派生自 `IObject`。下面是对该类的各个成员函数的简要说明：

### 成员函数：

1. **`getLayer` 函数：**
   - 返回当前图层节点所属的图层 (`ILayer`)。

2. **`getParent` 函数：**
   - 返回当前图层节点的父节点，如果没有父节点则返回 `nullptr`。

3. **`createChildItemNode` 和 `destroyChildItemNode` 函数：**
   - `createChildItemNode`：创建并返回一个子图层节点。
   - `destroyChildItemNode`：销毁指定的子图层节点。

4. **`upChildItemNode` 函数：**
   - 将指定的子图层节点置于其兄弟节点之上，使其在绘制和捕捉操作中处于更高的位置。

5. **`getEnumerator` 函数：**
   - 返回一个枚举器，用于遍历当前图层节点的子节点。

6. **`getLayerNodeCount` 和 `getLayerNodeAt` 函数：**
   - `getLayerNodeCount`：返回当前图层节点的子节点数量。
   - `getLayerNodeAt`：返回指定索引位置的子图层节点。

7. **`attachLayerItem` 和 `detachLayerItem` 函数：**
   - `attachLayerItem`：将图层项 (`ILayerItem`) 附加到当前图层节点。
   - `detachLayerItem`：从当前图层节点分离指定的图层项。

8. **`addToRenderItem` 函数：**
   - 向当前图层节点添加子项并返回对应的渲染项 (`RenderItem`)。参数包括纹理 (`ITexture`)，绘制队列信息和是否分离标志。

9. **`outOfDate` 函数：**
   - 标记指定的渲染项需要更新。

10. **`getLayerItemByPoint` 函数：**
    - 根据给定的屏幕坐标点返回位于该位置的图层项 (`ILayerItem`)。

11. **`renderToTarget` 函数：**
    - 将图层节点渲染到指定的渲染目标 (`IRenderTarget`)，并根据 `_update` 参数执行相应的更新操作。

12. **`resizeView` 函数：**
    - 调整视图大小，以适应新的视图尺寸。

13. **`getNodeDepth` 函数：**
    - 返回图层节点的深度值。

### 总体概述：

`ILayerNode` 是一个抽象类，用于表示图层系统中的节点。它提供了一组接口，用于管理图层节点的层次结构、子节点、图层项的附加和分离、渲染等操作。这样的设计允许实现特定图形引擎的图层节点，并与其他组件（如渲染项和图层项）进行交互。


### 没有 MyGUI_ILayerNode.cpp