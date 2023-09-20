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

