
git 查看别的分支的 log
```
# jjchen @ jjchen-ubuntu in ~/projects/byd_zros_android on git:android_sdk_di30_0522 x [10:05:13] C:141
$ git log android_sdk_di30
```

history
```
6439  git log
6440  git status
6441  git checkout .
6442  git status
6443  git branch -vv
6444  git checkout -b android_sdk_di30_0522
6446  git branch --set-upstream-to=origin/android_sdk_di30
6447  git branch -vv

6452  git reset --hard c2f659206a2766237e59cd7bae89f3304984ceab
6453  git log
6454  git status
6455  git log

6456  git log android_sdk_di30

6457  git cherry-pick a02c67b43f22a797db1b977b815baffd03ab07a8

6460  git apply --reject ./avm3d_app/out/0002-fix-autocalib-update-autocalib-config-for-byd-k526d.patch
6462  git add projects/hardware/car_instance/res/byd_kd_526d/autocalib/autocalib_param.xml
6463  git add projects/prebuilt/android_aarch64/autocalib/autocalib/lib/libautocalib.a
6464  git add projects/prebuilt/android_x86_64/autocalib/autocalib/lib/libautocalib.a
6466  git commit -m "0002"

6468  git apply --reject ./avm3d_app/out/0001-fix-autocalib-udpate-autocalib-configs-for-k526d-tia.patch
6470  git add projects/hardware/car_instance/res/byd_kd_526d
6472  git add projects/prebuilt/
6474  git commit -m "0001"
```