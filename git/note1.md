### git pull 更新最新

To update your local Git repository with the latest changes from the remote repository, you can use the `git pull` command. Here's how you can do it:

1. Open a terminal or command prompt.

2. Navigate to the directory of your local Git repository using the `cd` command.

3. Run the following command to pull the latest changes from the remote repository:
   
   ```bash
   git pull origin master
   ```
   
   In this command, `origin` is the name of the remote repository (you might have a different remote name) and `master` is the name of the branch you want to pull changes from. Make sure to replace `master` with the appropriate branch name if you're working with a different branch.

4. Git will fetch the latest changes from the remote repository and merge them into your current branch. If there are conflicts, you might need to resolve them manually.

After running `git pull`, your local repository will be updated with the latest changes from the remote repository.

Remember that before performing a `git pull`, it's a good practice to ensure that you don't have any uncommitted changes in your local repository. You can use `git status` to check the status of your working directory before pulling changes.

### git apply -R

The `git apply -R` command is used to reverse (unapply) a previously applied patch in a Git repository. This can be useful if you've applied a patch using `git apply` and want to undo the changes introduced by that patch.

Here's how to use `git apply -R`:

1. **Apply the Patch:**
   
   First, you need to have applied a patch using the `git apply` command. For example:
   
   ```sh
   git apply my_patch.patch
   ```

2. **Reverse the Applied Patch:**
   
   To reverse the applied patch, you can use the `git apply -R` command followed by the same patch file:
   
   ```sh
   git apply -R my_patch.patch
   ```
   
   This will attempt to revert the changes introduced by the patch. If the patch can be successfully reversed, the changes will be undone.

Keep in mind that reversing a patch might not always be successful if the repository has changed since the patch was originally applied. In complex scenarios, conflicts might arise during the reversal process, which you'll need to resolve manually.

Additionally, it's important to use version control practices to manage code changes effectively. Creating and applying patches is just one aspect of version control, and Git offers various other tools and workflows for managing changes in your repository.

### git push 被拒绝后，如何重新 git commit

如果在执行 `git push` 后被拒绝，通常是因为远程仓库中已经有了新的提交，而你的本地仓库落后于远程仓库。在这种情况下，你需要先将远程仓库的变更拉取到本地，然后解决冲突（如果有冲突），最后再次提交并推送。

以下是一般的步骤：

1. **拉取远程仓库变更**：使用 `git pull origin <branch-name>` 命令从远程仓库拉取变更到本地分支。这可能会导致合并冲突，需要解决冲突后再继续。

2. **解决冲突**：如果在拉取远程变更时发生冲突，需要手动解决这些冲突。打开冲突文件，查看并编辑冲突部分，然后执行 `git add` 将冲突标记为已解决。

3. **重新提交**：使用 `git commit` 重新提交你的变更，包括解决冲突后的修改。

4. **推送变更**：执行 `git push origin <branch-name>` 将你的本地提交推送到远程仓库。如果之前的问题已经解决，此时应该可以成功推送了。

需要注意的是，推送之前最好确保你的本地代码是最新的，并且没有未提交的变更。如果之前的 `git push` 操作被拒绝，不要担心，通过解决冲突并重新提交，你应该能够将你的变更成功推送到远程仓库。

### git 重新 commit

如果你需要修改之前的提交消息或者变更，可以使用 `git commit --amend` 命令来重新提交。

以下是一般的步骤：

1. **修改代码或者提交消息**：首先，你可以修改你的代码或者修改之前的提交消息。

2. **暂存修改**：使用 `git add` 命令将你的修改暂存到暂存区。

3. **执行 `git commit --amend`**：运行以下命令来重新提交：
   
   ```bash
   git commit --amend
   ```
   
   这会打开一个编辑器，显示你之前的提交消息。你可以修改提交消息，保存并关闭编辑器。

4. **推送修改**：如果之前的提交还没有被推送到远程仓库，你可以直接推送修改：
   
   ```bash
   git push origin <branch-name>
   ```

需要注意的是，如果你修改了最新的提交，然后执行 `git commit --amend`，它会将修改合并到最新的提交中，而不会创建一个新的提交。这对于修复之前的提交消息或者变更非常有用。

如果你需要修改更早的提交，可能需要使用 `git rebase` 命令来变基和编辑提交。但在执行 `git rebase` 之前，需要注意仔细阅读 Git 文档或者教程，以免出现不希望的结果。

### Git commit模板配置

git在提交时如果需要以指定的格式提交，可以给git配置一个提交模板(.commit-template)，配置好后，每次git commit时都会将模板中的信息作为初始化信息自动填充到编辑器的提交信息中。

以命令的方式配置：

git config --global commit.template ~/.commit-template

在Git配置文件中配置： 在git配置文件的commit下添加一项template即可，

### gerrit server创建仓库

在Gerrit服务器上创建仓库需要执行一系列步骤。Gerrit是一个用于代码审查和协作的工具，它通常与Git一起使用。以下是在Gerrit服务器上创建仓库的一般步骤：

1. **登录到Gerrit服务器**：
   首先，您需要通过SSH登录到您的Gerrit服务器。您需要拥有适当的权限以创建仓库。

2. **使用Gerrit命令行工具**：
   Gerrit提供了一个名为`gerrit`的命令行工具，用于管理仓库和项目。您可以使用这个工具来创建仓库。要创建一个新仓库，使用以下命令：
   
   ```bash
   gerrit create-project project-name
   ```
   
   在上面的命令中，`project-name`应替换为您要创建的仓库的名称。

3. **配置仓库访问权限**：
   创建仓库后，您可能需要配置仓库的访问权限。这是通过Gerrit的`project.config`文件完成的，该文件定义了与仓库相关的权限和设置。您可以使用Gerrit的`access`命令来编辑`project.config`文件。
   
   ```bash
   gerrit access project-name
   ```
   
   这将打开一个文本编辑器，允许您编辑`project.config`文件。您可以添加用户、组和权限规则以定义谁可以访问仓库以及执行哪些操作。

4. **推送代码到仓库**：
   一旦仓库创建并配置好访问权限，您可以将代码推送到新仓库中。首先，克隆仓库：
   
   ```bash
   git clone ssh://username@your-gerrit-server:29418/project-name.git
   ```
   
   然后将您的代码添加到仓库并推送：
   
   ```bash
   cd project-name
   # 添加和提交代码
   git add .
   git commit -m "Initial commit"
   # 推送到仓库
   git push origin HEAD:refs/for/master
   ```
   
   注意，Gerrit通常要求您使用`refs/for/master`来提交更改以进行审查。

这些是一般创建仓库的步骤。请注意，您需要相应的权限来执行这些操作，通常需要管理员或项目维护者权限。具体的步骤和权限设置可能会根据您的Gerrit服务器配置和策略而有所不同。请根据您的环境和需求调整这些步骤。

### gerrit代码迁移

将代码从一个Gerrit服务器迁移到另一个Gerrit服务器通常涉及多个步骤，包括克隆、推送和配置。下面是一般的代码迁移流程：

**注意：在执行这些步骤之前，请确保您在目标Gerrit服务器上有适当的权限。**

1. **在目标Gerrit服务器上创建新项目**：
   
   在目标Gerrit服务器上创建一个新项目，以便将要迁移的代码存储在其中。您可以使用Gerrit的`create-project`命令来创建项目。
   
   ```bash
   ssh username@target-gerrit-server gerrit create-project new-project-name
   ```
   
   替换 `username` 为您的用户名，`target-gerrit-server` 为目标Gerrit服务器地址，以及 `new-project-name` 为您要创建的新项目的名称。

2. **克隆原始项目**：
   
   在您的本地机器上，克隆原始Gerrit项目的代码。使用类似以下的命令：
   
   ```bash
   git clone ssh://username@source-gerrit-server:29418/original-project-name
   ```
   
   请替换 `username` 为您的用户名，`source-gerrit-server` 为原始Gerrit服务器地址，以及 `original-project-name` 为原始项目的名称。

3. **将代码推送到新项目**：
   
   进入克隆的原始项目目录，然后将代码推送到新项目。使用以下命令：
   
   ```bash
   git push ssh://username@target-gerrit-server:29418/new-project-name HEAD:refs/for/master
   ```
   
   这将推送整个代码库到新项目。

4. **配置访问权限**：
   
   在目标Gerrit服务器上，您可能需要配置新项目的访问权限。这通常涉及编辑新项目的`project.config`文件，以确保正确的用户和组具有适当的权限。
   
   ```bash
   ssh username@target-gerrit-server gerrit access new-project-name
   ```
   
   这将允许您编辑权限规则。

5. **同步更多数据（可选）**：
   
   如果您需要迁移更多的数据，如问题、标签或配置，您可能需要手动配置这些数据。具体的迁移细节会根据您的迁移需求而有所不同。

请注意，这是一个基本的代码迁移流程，实际迁移可能会更复杂，具体取决于您的环境和需求。迁移前，建议备份原始项目和数据，以防不时之需。

### git push --mirror

`git push --mirror` 是一个用于在Git中进行镜像推送的命令。它的主要作用是将一个仓库的所有分支、标签、提交历史以及引用都完全复制到另一个仓库，使得目标仓库成为源仓库的镜像副本，包括删除目标仓库中已不存在的分支和标签。

这个命令通常用于以下情况：

1. **克隆或备份仓库**：您想要创建一个源仓库的完全镜像，包括分支、标签和提交历史。这在创建仓库的备份或者在另一个地方部署仓库时非常有用。

2. **迁移仓库**：当您需要将一个Git仓库从一个Git服务器迁移到另一个Git服务器时，`git push --mirror` 可以帮助您在不丢失任何历史记录的情况下迁移整个仓库。

3. **合并或分离仓库**：如果您有两个仓库，想要将它们合并成一个，或者将一个仓库分离成两个，`git push --mirror` 可以帮助您进行这些操作。

请注意，`git push --mirror` 是一个强制性操作，它会覆盖目标仓库的内容。因此，在使用这个命令之前，务必小心谨慎，确保您了解其影响，并在必要的情况下备份目标仓库的内容。

### git 迁移某个 tag 到新项目

要将 Git 仓库中的某个标签（tag）迁移到新的项目，您可以按照以下步骤操作：

1. **克隆目标新项目**：
   
   首先，克隆新项目的 Git 仓库，以便将标签迁移到这个新项目中。假设您已经有了新项目的 Git 仓库，或者您可以在 Git 服务器上创建一个新项目。
   
   ```bash
   git clone ssh://username@new-gerrit-server:29418/new-project.git
   ```
   
   请替换 `username` 为您的用户名，`new-gerrit-server` 为新的 Gerrit 服务器地址，以及 `new-project` 为新项目的名称。

2. **切换到新项目的目录**：
   
   进入新项目的目录，准备将标签迁移到新项目中。
   
   ```bash
   cd new-project
   ```

3. **从旧项目中提取标签**：
   
   现在，您需要从旧项目中提取标签。首先，确保您具有旧项目的访问权限。使用以下命令从旧项目中提取标签：
   
   ```bash
   git fetch ssh://username@old-gerrit-server:29418/old-project.git refs/tags/<tag-name>:refs/tags/<tag-name>
   ```
   
   请替换 `username` 为您的用户名，`old-gerrit-server` 为旧的 Gerrit 服务器地址，`old-project` 为旧项目的名称，以及 `<tag-name>` 为您要   迁移的标签名称。

4. **推送标签到新项目**：
   
   最后，将刚刚提取的标签推送到新项目。
   
   ```bash
   git push origin refs/tags/<tag-name>:refs/tags/<tag-name>
   ```
   
   这将把标签推送到新项目，使其在新项目中可用。

请确保在执行这些操作之前，您在新项目和旧项目上都有适当的权限，并且仔细检查操作，以避免不必要的数据丢失或错误。如果在 Git 服务器上使用 Gerrit 等代码审查工具，还需要确保相关的代码审查流程也能够适应标签的迁移。

### git hook

Git hooks是Git版本控制系统中的脚本，允许您在特定的Git事件（如提交、推送、合并等）发生时自动触发自定义操作。这些钩子可以帮助您执行自动化任务、强制规则或添加自定义验证，以确保代码质量和工作流程的一致性。Git hooks存储在`.git/hooks`目录下，并具有特定的命名约定。

以下是一些常见的Git hooks：

1. **pre-commit**：在提交代码之前触发。可以用于执行代码风格检查、单元测试等。如果脚本返回非零退出代码，将阻止提交。

2. **pre-receive**：在远程Git服务器接收数据之前触发，通常用于执行服务器端的验证，如权限检查和分支命名规则。

3. **post-receive**：在远程Git服务器接收数据后触发，可用于自动化部署或通知。

4. **prepare-commit-msg**：在提交消息被编辑之前触发，可以用于自动添加信息到提交消息中，如关联的任务号或分支名称。

5. **pre-push**：在推送操作之前触发，可以用于执行本地验证和测试。

6. **post-merge**：在合并操作后触发，可用于自动执行一些后处理任务。

7. **post-checkout**：在检出分支后触发，可用于执行一些与分支切换相关的操作。

Git hooks是以脚本的形式存在的，通常是可执行的Shell脚本，但也可以是其他可执行程序。它们允许您自定义Git工作流程，以适应项目的需要。

Git hooks的脚本文件都位于`.git/hooks`目录下，您可以根据需要创建、编辑和管理这些脚本。要启用或禁用Git hooks，只需编辑或删除相关脚本文件。每个Git hook脚本有特定的参数和环境变量，这些参数和变量会根据事件类型而有所不同。您可以查看Git文档以获取有关每个Git hook的更多详细信息和用法。

### git fetch

`git fetch` 是 Git 命令之一，它用于从远程仓库获取（拉取）最新的变更，但不会自动合并这些变更到当前工作分支。相比之下，`git pull` 命令将获取并合并远程分支的变更。

具体来说，`git fetch` 的主要功能包括：

1. **获取远程分支的更新**：`git fetch` 会将远程仓库的变更（新提交、分支、标签等）下载到本地，但不会自动更新工作目录或当前分支。

2. **查看远程仓库状态**：`git fetch` 可以让您查看远程仓库的状态，以了解是否有新的提交或分支可用。

3. **不引起合并冲突**：由于 `git fetch` 不会自动合并变更，因此它不会引起合并冲突，可以帮助您在确保没有冲突的情况下查看远程仓库的变更。

使用示例：

```bash
git fetch origin
```

上述命令会从名为 "origin" 的远程仓库中获取最新的变更，但不会自动合并它们。要查看远程分支的状态，您可以运行以下命令：

```bash
git branch -r
```

这将显示远程分支列表，以便您了解哪些分支已经被更新。

如果要将获取的远程分支合并到当前工作分支，可以使用 `git pull` 命令：

```bash
git pull origin your-branch-name
```

这将从远程 `origin` 获取 `your-branch-name` 分支的变更并自动合并到当前工作分支。如果您只想查看变更而不进行合并，`git fetch` 是一个更安全的选项。

### git push origin refs/tags/V1.2.1:refs/tags/V1.2.1

`git push origin refs/tags/V1.2.1:refs/tags/V1.2.1` 是一个 Git 命令，用于将指定的标签（tag）从本地仓库推送到远程仓库。这个命令的语法如下：

```bash
git push <remote-name> <local-tag>:<remote-tag>
```

- `<remote-name>` 是远程仓库的名称（通常为 "origin"）。
- `<local-tag>` 是本地仓库中的标签名称。
- `<remote-tag>` 是远程仓库中的标签名称。

具体来说，`git push origin refs/tags/V1.2.1:refs/tags/V1.2.1` 的作用是将本地标签 `V1.2.1` 推送到远程仓库（通常是 "origin"）中，并使用相同的标签名称 `V1.2.1`。

这个操作将在远程仓库中创建或更新标签 `V1.2.1`，使得它与本地仓库中的标签保持一致。如果标签 `V1.2.1` 在远程仓库中已存在，它将被更新为与本地标签相匹配的状态。如果标签 `V1.2.1` 在远程仓库中不存在，它将被创建。

请注意，这个命令是单向操作，从本地推送到远程，不会影响本地标签的状态。如果您需要删除远程标签，可以使用 `git push origin --delete <remote-tag>` 命令。

### git 查看当前的 tag

要查看当前仓库中的标签（tags），可以使用以下 Git 命令：

```bash
git tag
```

这将列出仓库中所有已存在的标签。标签通常是用来标记特定的提交（commit）或版本，以便更容易地引用和管理项目的不同版本。

如果您想查看特定标签的详细信息，包括它指向的提交和标签消息，可以使用以下命令：

```bash
git show <tag-name>
```

请将 `<tag-name>` 替换为您要查看的标签的名称。这将显示与该标签关联的提交信息以及标签消息（如果有的话）。

如果您想查看远程仓库中的标签，可以运行以下命令：

```bash
git ls-remote --tags origin
```

这将显示远程仓库中的标签列表。

请注意，标签通常用于标记发布的版本，以便更容易地在不同的提交之间进行导航和跟踪。标签不像分支（branches）那样会自动移动，它们是静态的，通常不随新提交的创建而变化。

### git push origin HEAD:refs/heads/master

这个 `git push` 命令的作用是将当前分支（`HEAD`）的内容推送到远程仓库的 `master` 分支。具体地说，它将本地当前分支的内容（`HEAD` 所指向的分支）推送到远程仓库的 `master` 分支上。

让我们对这个命令的各部分进行解释：

- `git push origin`：这是推送操作的一部分，其中 `origin` 是远程仓库的名称。通常，`origin` 是默认的远程仓库名称，但您也可以替换它为您的远程仓库的名称。

- `HEAD`：这是一个特殊的Git引用，它指向当前所在的分支或提交。在这种情况下，`HEAD` 指向的是您当前所在的分支。

- `:`：这是推送操作的分隔符，它将本地分支和远程分支分开。

- `refs/heads/master`：这是远程分支的引用，`master` 是远程仓库中的分支名称。

因此，上述命令将当前分支的内容推送到远程仓库中的 `master` 分支。这对于更新远程 `master` 分支以匹配本地分支非常有用。请确保您在执行此操作之前已经在本地分支上进行了所需的更改和提交。

在这个过程中，如果远程 `master` 分支已经存在，您的更改将被合并到该分支中。如果远程 `master` 分支不存在，它将被创建，并将包含您当前分支的内容。在这两种情况下，确保您具有推送权限以执行此操作。

### 上述操作结果是：! [rejected]        HEAD -> master (non-fast-forward)

这个错误消息 `! [rejected] HEAD -> master (non-fast-forward)` 意味着在尝试将本地分支（HEAD）推送到远程 `master` 分支时，发生了拒绝，因为它不是一个“fast-forward” 操作。

Fast-forward 合并是指将一个分支的更改合并到另一个分支，而没有需要合并的冲突。在Git中，分支合并通常是Fast-forward合并，这是因为Git可以轻松地将更改应用到目标分支。

这个错误通常出现在以下情况下：

1. 远程 `master` 分支已经包含了其他提交，而您尝试将一个不包括这些提交的本地分支推送到远程 `master`。

2. 多个开发者同时尝试推送到同一个远程分支，其中一个人已经推送了更改，导致冲突。

解决这个问题的方法通常是执行一个合并或者将本地分支的更改重新基于远程 `master` 分支。以下是一些可能的解决方法：

1. **合并远程更改**：首先，确保您已经获取了远程 `master` 分支的最新更改，然后在本地分支上执行合并操作：
   
   ```bash
   git fetch origin master
   git merge origin/master
   ```
   
   这将合并远程 `master` 分支的更改到本地分支。

2. **重新基于远程分支**：如果您想保持线性历史并且没有合并冲突，可以使用 `rebase` 操作：
   
   ```bash
   git fetch origin master
   git rebase origin/master
   ```
   
   这将重新基于远程 `master` 分支，将您的更改应用在其之上。

3. **强制推送**（谨慎使用）：如果您确切知道自己的更改应该覆盖远程 `master` 分支，可以使用 `--force` 选项来执行强制推送，但这将覆盖远程分支并可能导致数据丢失：
   
   ```bash
   git push origin HEAD:master --force
   ```
   
   注意，谨慎使用强制推送，因为它会覆盖远程分支，潜在地导致数据丢失。

在任何情况下，请确保您了解更改如何影响您的项目和团队，并与团队成员协调推送操作，以避免数据丢失和合并冲突。

### fatal: the remote end hung up unexpectedly

参考：https://blog.csdn.net/weixin_38450840/article/details/80701173

在使用git更新或提交项目时候出现 "fatal: The remote end hung up unexpectedly " 原因是推送的文件太大。

方法一：

修改提交缓存大小为500M，或者更大的数字

git config --global http.postBuffer 524288000

// some comments below report having to double the value:

git config --global http.postBuffer 1048576000

或者在克隆/创建版本库生成的 .git目录下面修改生成的config文件增加如下：

```
[http]  
postBuffer = 524288000
```