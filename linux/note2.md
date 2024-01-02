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


## dotnet

`.NET` 是一个跨平台、开源、通用的软件框架，用于构建各种类型的应用程序。`.NET` 包括两个主要的组件: `.NET Runtime` 和 `.NET SDK`。

1. **.NET Runtime**:
   - 运行时环境，负责执行.NET应用程序。它包括 Common Language Runtime (CLR)，负责管理代码执行，以及其他运行时组件。

2. **.NET SDK (Software Development Kit)**:
   - 包含开发.NET应用程序所需的工具和资源。它提供了命令行工具（如 `dotnet` 命令），用于构建、运行和发布.NET应用程序。

`.NET` 支持多种编程语言，其中最常用的是 `C#`。

### 安装 .NET SDK

要在你的计算机上安装 `.NET SDK`，请按照以下步骤进行：

1. **在官方网站上下载 .NET SDK**:
   - 访问 [https://dotnet.microsoft.com/download](https://dotnet.microsoft.com/download)。
   - 选择你的操作系统（Windows、macOS 或 Linux）和架构（x64、x86 或 ARM）。
   - 下载并安装最新版本的 `.NET SDK`。

2. **验证安装**:
   - 打开终端或命令提示符。
   - 运行以下命令来验证安装：

     ```bash
     dotnet --version
     ```

     如果成功安装，将显示安装的 `.NET SDK` 版本号。

### 使用 .NET 命令行工具

`.NET` 提供了一组命令行工具，用于创建、构建、运行和测试应用程序。以下是一些常用的命令：

- **创建新项目**:
  
  ```bash
  dotnet new console -n MyConsoleApp
  ```

  这将创建一个名为 `MyConsoleApp` 的新控制台应用程序。

- **构建项目**:
  
  ```bash
  dotnet build
  ```

  这将编译项目。

- **运行项目**:

  ```bash
  dotnet run
  ```

  这将运行项目。

- **发布项目**:

  ```bash
  dotnet publish -c Release
  ```

  这将发布项目，生成用于部署的可执行文件。

这只是一些基本的命令，`.NET` 提供了许多其他命令和功能，可用于开发不同类型的应用程序。你可以通过运行 `dotnet --help` 获取帮助信息，了解所有可用的命令。


## ubuntu20.04 install dotnet

在 Ubuntu 20.04 上安装 .NET SDK 可以通过以下步骤完成。请注意，这是通过官方的 `.NET SDK` 安装脚本进行的安装。

1. **更新软件包列表**：

   ```bash
   sudo apt update
   ```

2. **安装依赖项**：

   ```bash
   sudo apt install -y apt-transport-https wget
   ```

3. **添加 Microsoft 的包签名密钥**：

   ```bash
   wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > microsoft.asc.gpg
   sudo mv microsoft.asc.gpg /etc/apt/trusted.gpg.d/
   ```

4. **添加 Microsoft 的包存储库**：

   ```bash
   sudo sh -c 'echo "deb [arch=amd64] https://packages.microsoft.com/repos/microsoft-ubuntu-focal-prod focal main" > /etc/apt/sources.list.d/dotnet.list'
   ```

   上述命令中的 `[arch=amd64]` 适用于 64 位系统。如果你在运行 32 位系统，请将其替换为 `[arch=i386]`。

5. **更新软件包列表**：

   ```bash
   sudo apt update
   ```

6. **安装 .NET SDK**：

   ```bash
   sudo apt install -y dotnet-sdk-5.0
   ```

   这将安装 .NET 5.0 SDK。如果你想安装其他版本，可以将 `dotnet-sdk-5.0` 替换为所需版本的包名。

7. **验证安装**：

   运行以下命令来验证 `.NET SDK` 是否正确安装：

   ```bash
   dotnet --version
   ```

   如果一切正常，将显示安装的 `.NET SDK` 版本号。

以上步骤应该能够在 Ubuntu 20.04 上成功安装 .NET SDK。


## ubuntu20.04 interface文件 
在 Ubuntu 20.04 中，网络接口的配置信息通常存储在 `/etc/netplan/` 目录下的 YAML 文件中。Ubuntu 20.04 使用 Netplan 工具来管理网络配置。

以下是一个示例 Netplan 配置文件：

```yaml
# /etc/netplan/01-netcfg.yaml

network:
  version: 2
  renderer: networkd
  ethernets:
    eth0:
      dhcp4: true
```

上述配置文件的含义是使用 DHCP（动态主机配置协议）为 `eth0` 网络接口分配 IP 地址。

你可以根据你的网络需求修改配置文件。例如，如果你想为接口手动配置静态 IP 地址，可以按照以下方式修改配置：

```yaml
# /etc/netplan/01-netcfg.yaml

network:
  version: 2
  renderer: networkd
  ethernets:
    eth0:
      addresses: [192.168.1.2/24]
      gateway4: 192.168.1.1
      nameservers:
        addresses: [8.8.8.8, 8.8.4.4]
```

在上面的示例中，`eth0` 接口被手动配置为静态 IP 地址（192.168.1.2），子网掩码为 `/24`，网关为 192.168.1.1，DNS 服务器为 Google 的公共 DNS 服务器（8.8.8.8 和 8.8.4.4）。

修改配置后，你需要应用配置：

```bash
sudo netplan apply
```

这将使新的网络配置生效。确保在修改配置文件之前备份它，以免出现配置错误导致网络连接问题。


### 如何将 enxc6678d834178: flags=4163<UP,BROADCAST,RUNNING,MULTICAST> ... 添加到 Netplan 配置文件

要将网络接口 `enxc6678d834178` 添加到 Netplan 配置文件，你需要编辑 `/etc/netplan/` 目录下的一个 `.yaml` 文件。通常，Netplan 配置文件的名称以数字开头，例如 `01-netcfg.yaml`。

以下是一个示例 Netplan 配置文件，将 `enxc6678d834178` 接口配置为使用静态 IP 地址：

```yaml
# /etc/netplan/01-netcfg.yaml

network:
  version: 2
  renderer: networkd
  ethernets:
    enxc6678d834178:
      addresses:
        - 192.168.1.2/24
      gateway4: 192.168.1.1
      nameservers:
        addresses: [8.8.8.8, 8.8.4.4]
```

请注意以下事项：

- `version: 2` 表明使用 Netplan 配置版本 2。
- `renderer: networkd` 表示使用 systemd-networkd 渲染器。
- `enxc6678d834178` 是你的网络接口的名称，记得替换为你实际的接口名称。
- `addresses` 包含你要为接口配置的 IP 地址和子网掩码。
- `gateway4` 是默认网关的 IP 地址。
- `nameservers` 包含 DNS 服务器的 IP 地址。

配置完成后，保存文件并运行以下命令应用新的网络配置：

```bash
sudo netplan apply
```

请注意，在修改配置文件之前，请确保备份以防出现配置错误导致网络连接问题。配置文件中的具体内容应该根据你的网络需求进行调整。