### ubunty如何开机自动mount 加密的硬盘
要让Ubuntu在开机时自动挂载加密的硬盘，你可以按照以下步骤操作：

1. **创建挂载点：** 首先，创建一个空文件夹作为挂载点。你可以选择在系统中的任何地方创建这个文件夹，比如 `/mnt/encrypted_drive`。

   ```bash
   sudo mkdir /mnt/encrypted_drive
   ```

2. **编辑 /etc/crypttab 文件：** 使用文本编辑器打开 `/etc/crypttab` 文件，并添加一个条目来定义你的加密硬盘。条目的格式如下：

   ```
   <名称> <源设备> <密钥文件> <选项>
   ```

   - `<名称>`：加密设备的名称，可以自定义。
   - `<源设备>`：加密设备的路径或UUID。
   - `<密钥文件>`：密钥文件的路径（如果有的话）。
   - `<选项>`：挂载选项，通常为 `none`。

   例如，假设你的加密设备是 `/dev/sdb1`，可以添加如下条目：

   ```
   encrypted_drive /dev/sdb1 none luks
   ```

3. **编辑 /etc/fstab 文件：** 使用文本编辑器打开 `/etc/fstab` 文件，并添加一个条目来定义挂载点。条目的格式如下：

   ```
   /dev/mapper/<名称> <挂载点> <文件系统类型> <挂载选项> <dump> <fsck>
   ```

   - `/dev/mapper/<名称>`：加密设备的映射路径，通常在 `/dev/mapper` 目录下。
   - `<挂载点>`：之前创建的挂载点路径。
   - `<文件系统类型>`：加密设备中存储的文件系统类型，比如 ext4。
   - `<挂载选项>`：挂载时使用的选项，比如 `defaults`。
   - `<dump>` 和 `<fsck>`：通常为 `0`，表示不进行备份和文件系统检查。

   例如，可以添加如下条目：

   ```
   /dev/mapper/encrypted_drive /mnt/encrypted_drive ext4 defaults 0 0
   ```

4. **测试挂载：** 重启系统或者手动执行以下命令以测试挂载：

   ```bash
   sudo mount -a
   ```

   如果没有报错，说明挂载成功。

5. **重启系统：** 重新启动系统，确保加密硬盘能够在系统启动时自动挂载。

完成以上步骤后，加密的硬盘应该会在系统开机时自动挂载到指定的挂载点。请确保在编辑配置文件时谨慎操作，以免造成系统不稳定或数据丢失。

### 请详细描述 编辑 /etc/crypttab 文件
`/etc/crypttab` 文件用于配置加密设备的信息，包括设备名称、源设备、密钥文件路径和挂载选项。下面是对`/etc/crypttab` 文件的详细描述和示例：

1. **打开文件：** 打开终端，并使用任何文本编辑器（如nano或vim）以root权限打开`/etc/crypttab` 文件：

   ```bash
   sudo nano /etc/crypttab
   ```

2. **文件格式：** `crypttab` 文件的每一行都描述了一个加密设备。每行包含四个字段，用空格分隔：

   ```
   <名称> <源设备> <密钥文件> <选项>
   ```

   - `<名称>`：加密设备的名称，可以自定义。
   - `<源设备>`：加密设备的路径或UUID。
   - `<密钥文件>`：密钥文件的路径（如果有的话）。
   - `<选项>`：挂载选项，通常为 `none`。

3. **示例：** 以下是一个示例`/etc/crypttab` 文件的内容：

   ```
   # <名称> <源设备> <密钥文件> <选项>
   encrypted_drive /dev/sdb1 none luks
   ```

   在这个示例中：
   - `<名称>` 是 `encrypted_drive`，表示加密设备的名称。
   - `<源设备>` 是 `/dev/sdb1`，表示加密设备的路径。
   - `<密钥文件>` 是 `none`，表示没有指定密钥文件。
   - `<选项>` 是 `luks`，表示加密设备使用的是LUKS（Linux Unified Key Setup）加密。

4. **保存文件：** 在编辑完成后，按下 `Ctrl + X`，然后输入 `Y` 确认保存文件，最后按下 `Enter` 退出编辑器。

5. **测试配置：** 编辑完`/etc/crypttab` 文件后，可以使用以下命令测试配置是否正确：

   ```bash
   sudo cryptdisks_start encrypted_drive
   ```

   如果没有报错，说明配置正确。

6. **重启系统：** 重新启动系统，确保加密设备能够在系统启动时自动解锁并挂载。

通过编辑`/etc/crypttab` 文件，你可以方便地配置加密设备的信息，以便在系统启动时自动解锁和挂载这些设备。


### mount: /SSD: special device /dev/mapper/encrypted_drive does not exist.
这个错误表明 `/dev/mapper/encrypted_drive` 设备不存在，可能是因为加密设备没有正确解锁或者没有成功映射。

在解决这个问题之前，有几个步骤可以进行检查和确认：

1. **确认加密设备已解锁：** 使用 `cryptsetup luksOpen` 命令手动解锁加密设备，并检查是否成功。如果没有成功解锁，可能需要确认密钥是否正确或者设备是否已经正确初始化。

   ```bash
   sudo cryptsetup luksOpen /dev/sdb1 encrypted_drive
   ```

2. **检查 /etc/crypttab 文件：** 确保 `/etc/crypttab` 文件中的条目正确配置，并且名称、源设备和密钥文件路径都是正确的。

3. **重新生成 initramfs：** 如果修改了 `/etc/crypttab` 文件，请确保重新生成 initramfs，以确保内核正确加载加密模块和设备映射。

   ```bash
   sudo update-initramfs -u
   ```

4. **查看 dmesg 输出：** 使用 `dmesg` 命令查看系统日志，寻找有关加密设备解锁和映射的错误信息，以便更详细地了解问题的原因。

一旦你确认加密设备已经成功解锁并且 `/etc/crypttab` 文件配置正确，应该就能够正常挂载加密设备了。


### 如果自动输入 密钥文件

如果要在自动解锁加密设备时使用密钥文件，你可以将密钥文件的路径添加到 `/etc/crypttab` 文件中的相应条目中。下面是如何做的：

1. **编辑 `/etc/crypttab` 文件：** 打开 `/etc/crypttab` 文件，找到你之前添加的条目，并在第三个字段中指定密钥文件的路径。例如：

   ```
   encrypted_drive /dev/sdb1 /path/to/keyfile luks
   ```

   在这个例子中，`/path/to/keyfile` 是密钥文件的路径。确保密钥文件的权限设置为只能由 root 用户读取，以确保安全性。

2. **保存文件：** 保存并关闭 `/etc/crypttab` 文件。

3. **测试配置：** 使用 `sudo cryptdisks_start` 命令测试配置是否正确。系统应该会自动使用密钥文件来解锁加密设备。

   ```bash
   sudo cryptdisks_start encrypted_drive
   ```

4. **重启系统：** 重新启动系统，确保加密设备能够在系统启动时自动解锁并挂载，而无需手动输入密钥文件。

通过在 `/etc/crypttab` 文件中指定密钥文件的路径，你可以实现在系统启动时自动解锁加密设备，而无需手动输入密钥。请确保密钥文件的安全性，并在设置密钥文件路径时谨慎操作。


### LUKS Encryption 自动开机解密的密钥文件
LUKS（Linux Unified Key Setup）加密通常允许用户使用密钥文件来自动解锁加密设备。以下是设置和使用密钥文件进行自动解锁的步骤：

1. **生成密钥文件：** 首先，你需要生成一个随机的密钥文件。你可以使用 `dd` 命令生成一个随机的二进制文件作为密钥文件。

   ```bash
   sudo dd if=/dev/urandom of=/root/luks_keyfile bs=512 count=4
   ```

   这会在 `/root/` 目录下生成一个名为 `luks_keyfile` 的密钥文件。

2. **将密钥文件添加到加密设备中：** 现在，你需要将生成的密钥文件添加到加密设备中。你可以使用 `cryptsetup luksAddKey` 命令来添加密钥文件。

   ```bash
   sudo cryptsetup luksAddKey /dev/sdX /root/luks_keyfile
   ```

   其中 `/dev/sdX` 是你的加密设备的名称。

3. **编辑 /etc/crypttab 文件：** 然后，你需要编辑 `/etc/crypttab` 文件，以指定使用密钥文件来解锁加密设备。在该文件中，你需要添加一个条目，指定加密设备、密钥文件以及其他选项。

   ```plaintext
   my_encrypted_device /dev/sdX /root/luks_keyfile luks
   ```

   其中 `my_encrypted_device` 是你为加密设备指定的名称，`/dev/sdX` 是加密设备的路径，`/root/luks_keyfile` 是密钥文件的路径，`luks` 是加密类型。

4. **更新 initramfs：** 最后，你需要更新 initramfs 以确保系统在引导时能够识别并使用密钥文件解锁加密设备。

   ```bash
   sudo update-initramfs -u
   ```

   这会重新生成 initramfs，包括密钥文件，以便系统能够在引导时自动解锁加密设备。

完成以上步骤后，加密设备应该在系统引导时自动解锁，并且不需要手动输入密码。请注意，密钥文件应该被妥善保管，并且只有授权的用户才能访问它。


### /etc/crypttab 相关内容
```
encrypted_drive /dev/sda1 /root/luks_keyfile luks
```

### /etc/fstab  相关内容
```
/dev/mapper/encrypted_drive /SSD ext4 defaults 0 0
```