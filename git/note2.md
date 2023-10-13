### repo 命令

`repo`命令是用于管理Android源代码库的工具，尤其是针对基于Android开源项目（AOSP）的项目。它简化了在Android开发中处理多个Git仓库的过程。以下是一些常见的`repo`命令：

1. **初始化一个新的Repo客户端**:
   
   ```
   repo init -u <清单库的URL> -b <分支> -m <清单文件> --repo-url=<repo仓库的URL>
   ```
   
   - `-u`：指定清单库的URL。
   - `-b`：指定要使用的清单分支。
   - `-m`：指定清单文件的名称。
   - `--repo-url`：指定`repo`仓库的URL。
   - #### repo init 出现 Permission denied 问题
     
     可以在 repo init 的 --repo-url 的字段加上用户名

2. **同步项目**:
   
   ```
   repo sync
   ```
   
   这个命令会将本地仓库与清单中指定的远程仓库同步。它会获取远程仓库的更新并将其更新到本地。

3. **列出项目**:
   
   ```
   repo list
   ```
   
   这个命令会列出项目，这些项目是Android源代码库的一部分。

4. **创建新功能分支**:
   
   ```
   repo start <分支名> <项目名>
   ```
   
   这会在指定项目中创建一个新的分支。

5. **放弃更改**:
   
   ```
   repo abandon <分支名>
   ```
   
   这个命令会放弃某个本地分支的更改。

6. **上传更改至Gerrit（代码审查）**:
   
   ```
   repo upload
   ```
   
   这个命令会将你的代码更改上传至Gerrit代码审查系统，以便其他人审查你的代码。

7. **创建本地Git分支**:
   
   ```
   repo start <分支名> --all
   ```
   
   这会在所有项目中创建一个本地Git分支。

8. **项目状态**:
   
   ```
   repo status
   ```
   
   这个命令显示本地项目的状态，指示哪些项目具有未提交的更改。

9. **下载特定项目**:
   
   ```
   repo sync <项目名>
   ```
   
   这个命令仅同步指定的项目，适用于不需要更新所有项目的情况。

10. **强制同步**:
    
    ```
    repo sync -f
    ```
    
    这个命令强制进行同步，覆盖任何本地更改。

这些命令只是你可以使用`repo`工具执行的众多示例。你使用的具体命令和清单结构取决于你的Android项目的结构和开发工作流程。

### ubuntu 使用  repo

在 Ubuntu 上使用 `repo` 工具来管理 Android 项目（特别是 Android 开源项目，AOSP）的源代码仓库是一个常见的做法。下面是在 Ubuntu 上使用 `repo` 的一般步骤：

1. **安装 Repo 工具**：
   
   在终端中运行以下命令以安装 Repo 工具：
   
   ```bash
   mkdir -p ~/bin
   curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
   chmod a+x ~/bin/repo
   ```
   
   这将创建一个 `bin` 目录并下载 Repo 工具到其中，并授予执行权限。

2. **设置环境变量**：
   
   为了让系统知道 Repo 工具的位置，你需要将 `bin` 目录添加到你的环境变量中。在你的 `~/.bashrc` 或 `~/.zshrc`（根据你使用的 shell）文件中添加以下行：
   
   ```bash
   export PATH=~/bin:$PATH
   ```
   
   然后，运行 `source ~/.bashrc` 或 `source ~/.zshrc` 来使更改生效。

3. **初始化 Repo**：
   
   在一个目录中创建一个新的 Android 项目目录，然后使用 `repo` 初始化它。例如，你可以运行以下命令：
   
   ```bash
   mkdir my_android_project
   cd my_android_project
   repo init -u <URL of manifest repository> -b <branch> -m <manifest file>
   ```
   
   这会使用 Repo 工具初始化你的 Android 项目，其中包含了一个清单（manifest）来定义所需的 Git 仓库和分支。

4. **同步源代码**：
   
   一旦初始化了 Android 项目，你可以使用 `repo sync` 命令来同步源代码：
   
   ```bash
   repo sync
   ```
   
   这将下载和更新你的 Android 项目中的所有 Git 仓库。

5. **使用 Repo 进行开发**：
   
   一旦源代码同步完成，你可以使用标准的 Git 命令来在项目的各个仓库中进行开发。在 Android 项目中，通常会使用 `repo start` 来创建新分支，然后进行开发。

这些是使用 Repo 工具在 Ubuntu 上管理 Android 项目的一般步骤。确保你有合适的清单（manifest）URL、分支和清单文件来初始化你的项目。

### repo init 出现 fatal: cannot exec 'ssh': Permission denied

The "fatal: cannot exec 'ssh': Permission denied" error typically occurs when the `ssh` executable does not have execute permission. This error might be due to a permissions issue on your system.

To resolve this issue, you can try the following steps:

1. **Check SSH Permissions**:
   
   Ensure that the `ssh` executable has the correct permissions. Open a terminal and run the following command to check the permissions:
   
   ```bash
   ls -l $(which ssh)
   ```
   
   It should have the execute permission set for the user. The output should look something like this:
   
   ```bash
   -rwxr-xr-x 1 user user 1234567890 /usr/bin/ssh
   ```
   
   If the execute permission is not set for the user (the "x" in `-rwxr-xr-x`), you can set it using the `chmod` command:
   
   ```bash
   sudo chmod +x $(which ssh)
   ```

2. **Check SSH Configuration**:
   
   Make sure that your SSH configuration is correctly set up. Ensure that your SSH key is correctly added to your SSH agent and that your SSH configuration (`~/.ssh/config`) does not contain any errors.

3. **Check User and Group Permissions**:
   
   Make sure that the user you're using to run `repo init` has the appropriate permissions. They should be the owner of the `.repo` directory and all the files and directories inside it.
   
   You can change the owner of the `.repo` directory and its contents to your user using the `chown` command:
   
   ```bash
   sudo chown -R your_username:your_username .repo
   ```
   
   Replace `your_username` with your actual username.

4. **Run repo init as a Normal User**:
   
   Ensure that you are running `repo init` as a normal user, not as the root user. Running it as the root user can sometimes lead to permission issues.

5. **Reboot Your System**:
   
   Sometimes, permission issues can be resolved by simply rebooting your system, especially if you've recently made changes to your user or group settings.

If you continue to encounter the "Permission denied" issue after following these steps, please provide more context about your system and what you were doing when you encountered the error so that I can provide more specific assistance.

### 修改 repo 的 python 最低版本

参考：https://blog.csdn.net/m0_37910557/article/details/120764109

实际上就是修改上述 `~/bin/repo` 的 `MIN_PYTHON_VERSION` ，将 python3 版本改成 python2 版本：

```
# MIN_PYTHON_VERSION_SOFT = (3, 6)
# MIN_PYTHON_VERSION_HARD = (3, 5)
MIN_PYTHON_VERSION_SOFT = (2, 7)
MIN_PYTHON_VERSION_HARD = (2, 7)
```

而且还需要修改 ubuntu 对应 python 的默认版本：

```
rm -rf  /usr/bin/python
ln -s /usr/bin/python2 /usr/bin/python
```

就可以正确进行 `repo init`。
