### MyGUI_LayerNode.h

这是一个 C++ 类的定义，名为 `LayerNode`，它实现了 `ILayerNode` 接口。以下是对该类的成员函数和成员变量的详细分析：

```cpp
class LayerNode : public ILayerNode
{
    MYGUI_RTTI_DERIVED(LayerNode)

public:
    // 构造函数，需要指定所属的图层和父节点（默认为nullptr）
    explicit LayerNode(ILayer* _layer, ILayerNode* _parent = nullptr);

    // 析构函数
    ~LayerNode() override;

    // 获取所属的图层
    ILayer* getLayer() const override;

    // 获取父节点，如果没有父节点则返回 nullptr
    ILayerNode* getParent() const override;

    // 创建并返回一个子图层节点
    ILayerNode* createChildItemNode() override;

    // 销毁指定的子图层节点
    void destroyChildItemNode(ILayerNode* _node) override;

    // 将指定的子图层节点置于其兄弟节点之上，使其在绘制和捕捉操作中处于更高的位置
    void upChildItemNode(ILayerNode* _item) override;

    // 返回一个枚举器，用于遍历当前图层的子节点
    EnumeratorILayerNode getEnumerator() const override;

    // 获取子节点的数量
    size_t getLayerNodeCount() const override;

    // 根据索引返回指定的子节点
    ILayerNode* getLayerNodeAt(size_t _index) const override;

    // 将图层项（ILayerItem）附加到当前节点
    void attachLayerItem(ILayerItem* _item) override;

    // 从当前节点分离指定的图层项
    void detachLayerItem(ILayerItem* _item) override;

    // 添加一个子项到渲染项，并返回渲染项
    RenderItem* addToRenderItem(ITexture* _texture, bool _firstQueue, bool _manualRender) override;

    // 标记当前节点为需要更新
    void outOfDate(RenderItem* _item) override;

    // 根据给定的位置返回位于该位置的图层项
    ILayerItem* getLayerItemByPoint(int _left, int _top) const override;

    // 将当前图层绘制到指定的渲染目标
    void renderToTarget(IRenderTarget* _target, bool _update) override;

    // 调整视图大小
    void resizeView(const IntSize& _viewSize) override;

    // 获取节点的深度
    float getNodeDepth() const override;

    // 检查当前节点是否需要更新
    bool isOutOfDate() const;

protected:
    // 将所有空缓冲区推送到缓冲区列表的末尾
    void updateCompression();

    // 将图层项添加到第一个渲染队列
    RenderItem* addToRenderItemFirstQueue(ITexture* _texture, bool _manualRender);

    // 将图层项添加到第二个渲染队列
    RenderItem* addToRenderItemSecondQueue(ITexture* _texture, bool _manualRender);

protected:
    // 两个渲染队列，用于子窗口和文本
    // 第一个队列保持基于创建顺序的渲染顺序
    // 第二个队列忽略创建顺序，并始终合并具有相同纹理的渲染项
    VectorRenderItem mFirstRenderItems;
    VectorRenderItem mSecondRenderItems;

    // 记录最后一个非空项的索引
    size_t mLastNotEmptyItem{0};

    // 根部件列表
    // 重叠的图层在这里只有一项
    VectorLayerItem mLayerItems;

    VectorILayerNode mChildItems;

    ILayerNode* mParent;
    ILayer* mLayer;
    bool mOutOfDate{false};
    bool mOutOfDateCompression{false};
    float mDepth{0.0f};
};
```

该类表示图层节点，用于管理图层中的元素。它继承自 `ILayerNode` 接口，并包含了一系列成员函数和成员变量，用于处理图层节点的创建、销毁、更新和渲染等操作。


### MyGUI_LayerNode.cpp

这是 `LayerNode` 类的实现部分。以下是对其中几个重要函数的详细分析：

1. **构造函数和析构函数：**
   ```cpp
   LayerNode::LayerNode(ILayer* _layer, ILayerNode* _parent) :
       mParent(_parent),
       mLayer(_layer)
   {
   }

   LayerNode::~LayerNode()
   {
       for (auto& firstRenderItem : mFirstRenderItems)
           delete firstRenderItem;
       mFirstRenderItems.clear();

       for (auto& secondRenderItem : mSecondRenderItems)
           delete secondRenderItem;
       mSecondRenderItems.clear();

       for (auto& childItem : mChildItems)
           delete childItem;
       mChildItems.clear();
   }
   ```
   构造函数用于初始化 `LayerNode` 对象，设置所属图层和父节点。析构函数用于清理该节点所拥有的资源，包括释放渲染项和子节点。

2. **创建和销毁子节点：**
   ```cpp
   ILayerNode* LayerNode::createChildItemNode()
   {
       LayerNode* layer = new LayerNode(mLayer, this);
       mChildItems.push_back(layer);

       mOutOfDate = true;

       return layer;
   }

   void LayerNode::destroyChildItemNode(ILayerNode* _node)
   {
       for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
       {
           if ((*iter) == _node)
           {
               delete _node;
               mChildItems.erase(iter);

               mOutOfDate = true;

               return;
           }
       }
       MYGUI_EXCEPT("item node not found");
   }
   ```
   `createChildItemNode` 用于创建子节点，并将其添加到当前节点的子节点列表中。`destroyChildItemNode` 用于销毁指定的子节点。

3. **更新渲染队列：**
   ```cpp
   RenderItem* LayerNode::addToRenderItem(ITexture* _texture, bool _firstQueue, bool _manualRender)
   {
       RenderItem* item = nullptr;
       if (_firstQueue)
           item = addToRenderItemFirstQueue(_texture, _manualRender);
       else
           item = addToRenderItemSecondQueue(_texture, _manualRender);

       mOutOfDate = false;
       return item;
   }
   ```
   `addToRenderItem` 用于将图层项添加到渲染队列中，根据 `_firstQueue` 参数选择添加到第一个队列还是第二个队列。

4. **渲染到目标：**
   ```cpp
   void LayerNode::renderToTarget(IRenderTarget* _target, bool _update)
   {
       mDepth = _target->getInfo().maximumDepth;

       if (mOutOfDateCompression)
       {
           updateCompression();
           mOutOfDateCompression = false;
       }

       // 先渲染当前节点的图层项
       for (auto& firstRenderItem : mFirstRenderItems)
           firstRenderItem->renderToTarget(_target, _update);

       for (auto& secondRenderItem : mSecondRenderItems)
           secondRenderItem->renderToTarget(_target, _update);

       // 再渲染子节点
       for (auto& childItem : mChildItems)
           childItem->renderToTarget(_target, _update);

       mOutOfDate = false;
   }
   ```
   `renderToTarget` 用于将当前节点及其子节点渲染到指定的渲染目标。该函数按照渲染顺序，先渲染当前节点的图层项，然后渲染子节点。

5. **获取屏幕坐标下的图层项：**
   ```cpp
   ILayerItem* LayerNode::getLayerItemByPoint(int _left, int _top) const
   {
       // 先检查子节点
       for (const auto& childItem : mChildItems)
       {
           ILayerItem* item = childItem->getLayerItemByPoint(_left, _top);
           if (nullptr != item)
               return item;
       }

       // 再检查当前节点的图层项
       for (const auto& layerItem : mLayerItems)
       {
           ILayerItem* item = layerItem->getLayerItemByPoint(_left, _top);
           if (nullptr != item)
               return item;
       }

       return nullptr;
   }
   ```
   `getLayerItemByPoint` 用于获取给定屏幕坐标下的图层项，首先检查子节点，然后检查当前节点的图层项。

以上是 `LayerNode` 类中几个关键函数的详细分析。该类的主要功能包括管理渲染队列、处理子节点、更新节点状态和渲染到目标等。