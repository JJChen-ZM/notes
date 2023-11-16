### VirtualBox 虚拟机动态磁盘扩容

- 首先扩容动态盘：https://zhuanlan.zhihu.com/p/319431032

- 然后扩容实际硬盘，涉及到新分区，还需要使用Gparted工具，教程：https://blog.csdn.net/benben0503/article/details/113865502

### 使用find查找包含指定字符串的文件

参考：https://blog.csdn.net/weixin_43937576/article/details/118515984

```
find -name "检索的文件名" | xargs grep -rin "指定的字符串"
```

检索当前路径下的包含指定字符串的文件

```
find . | xargs grep -rin "指定字符串"
```

grep命令
-r 递归查找
-i 不区分大小写
-n 显示行号

直接使用grep命令
grep -rn “_LITE_VERSION” ./