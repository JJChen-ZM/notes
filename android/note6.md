## 如何通过JNI获取 suspensionValue 的值
```
public static class CA_HMSG_USER_VEHICLE_INFO_NON_PERIODIC{
        public CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE suspensionValue;

        public CA_HMSG_USER_VEHICLE_INFO_NON_PERIODIC(){
        }
    }

    public static class CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE{
        public float VMCLvlTarFL;
        public float VMCLvlTarRR;
        public int CdcVMCEcasModeCHASts;
        public int VMCECASComplSts;

        public CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE(){
        }
    }
如何通过JNI获取 suspensionValue 的值
```


要通过 JNI 获取 Java 类 `CA_HMSG_USER_VEHICLE_INFO_NON_PERIODIC` 中的 `suspensionValue` 的值，我们需要在 JNI 代码中获取 `suspensionValue` 对象，然后分别访问它的字段 `VMCLvlTarFL`, `VMCLvlTarRR`, `CdcVMCEcasModeCHASts`, 和 `VMCECASComplSts`。

### 1. Java 类结构

确保 Java 类结构如下：

```java
public static class CA_HMSG_USER_VEHICLE_INFO_NON_PERIODIC {
    public CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE suspensionValue;

    public CA_HMSG_USER_VEHICLE_INFO_NON_PERIODIC() {
    }
}

public static class CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE {
    public float VMCLvlTarFL;
    public float VMCLvlTarRR;
    public int CdcVMCEcasModeCHASts;
    public int VMCECASComplSts;

    public CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE() {
    }
}
```

### 2. JNI C++ 代码

在 C++ 中使用 JNI 获取这些字段值的步骤如下：

```cpp
#include <jni.h>
#include <iostream>

// 声明 JNI 方法来获取 suspensionValue 的值
extern "C" JNIEXPORT void JNICALL
Java_com_example_Main_getSuspensionValue(JNIEnv *env, jobject obj, jobject vehicleInfoObj) {
    // 获取 CA_HMSG_USER_VEHICLE_INFO_NON_PERIODIC 类
    jclass vehicleInfoClass = env->GetObjectClass(vehicleInfoObj);
    if (vehicleInfoClass == NULL) {
        std::cerr << "Class CA_HMSG_USER_VEHICLE_INFO_NON_PERIODIC not found" << std::endl;
        return;
    }

    // 获取 suspensionValue 字段的 ID
    jfieldID suspensionValueFieldID = env->GetFieldID(vehicleInfoClass, "suspensionValue", "Lcom/example/CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE;");
    if (suspensionValueFieldID == NULL) {
        std::cerr << "Field 'suspensionValue' not found" << std::endl;
        return;
    }

    // 获取 CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE 对象
    jobject suspensionValueObj = env->GetObjectField(vehicleInfoObj, suspensionValueFieldID);
    if (suspensionValueObj == NULL) {
        std::cerr << "suspensionValue object is NULL" << std::endl;
        return;
    }

    // 获取 CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE 类
    jclass suspensionValueClass = env->GetObjectClass(suspensionValueObj);
    if (suspensionValueClass == NULL) {
        std::cerr << "Class CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE not found" << std::endl;
        return;
    }

    // 获取 VMCLvlTarFL 字段的 ID 并获取其值
    jfieldID VMCLvlTarFLFieldID = env->GetFieldID(suspensionValueClass, "VMCLvlTarFL", "F");
    if (VMCLvlTarFLFieldID == NULL) {
        std::cerr << "Field 'VMCLvlTarFL' not found" << std::endl;
        return;
    }
    jfloat VMCLvlTarFL = env->GetFloatField(suspensionValueObj, VMCLvlTarFLFieldID);

    // 获取 VMCLvlTarRR 字段的 ID 并获取其值
    jfieldID VMCLvlTarRRFieldID = env->GetFieldID(suspensionValueClass, "VMCLvlTarRR", "F");
    if (VMCLvlTarRRFieldID == NULL) {
        std::cerr << "Field 'VMCLvlTarRR' not found" << std::endl;
        return;
    }
    jfloat VMCLvlTarRR = env->GetFloatField(suspensionValueObj, VMCLvlTarRRFieldID);

    // 获取 CdcVMCEcasModeCHASts 字段的 ID 并获取其值
    jfieldID CdcVMCEcasModeCHAStsFieldID = env->GetFieldID(suspensionValueClass, "CdcVMCEcasModeCHASts", "I");
    if (CdcVMCEcasModeCHAStsFieldID == NULL) {
        std::cerr << "Field 'CdcVMCEcasModeCHASts' not found" << std::endl;
        return;
    }
    jint CdcVMCEcasModeCHASts = env->GetIntField(suspensionValueObj, CdcVMCEcasModeCHAStsFieldID);

    // 获取 VMCECASComplSts 字段的 ID 并获取其值
    jfieldID VMCECASComplStsFieldID = env->GetFieldID(suspensionValueClass, "VMCECASComplSts", "I");
    if (VMCECASComplStsFieldID == NULL) {
        std::cerr << "Field 'VMCECASComplSts' not found" << std::endl;
        return;
    }
    jint VMCECASComplSts = env->GetIntField(suspensionValueObj, VMCECASComplStsFieldID);

    // 输出获取的值
    std::cout << "VMCLvlTarFL: " << VMCLvlTarFL << std::endl;
    std::cout << "VMCLvlTarRR: " << VMCLvlTarRR << std::endl;
    std::cout << "CdcVMCEcasModeCHASts: " << CdcVMCEcasModeCHASts << std::endl;
    std::cout << "VMCECASComplSts: " << VMCECASComplSts << std::endl;
}
```

### 3. 代码解释

1. **获取类引用**：使用 `GetObjectClass` 获取 Java 类 `CA_HMSG_USER_VEHICLE_INFO_NON_PERIODIC` 和 `CA_HMSG_USER_VEHICLE_SUSPENSION_VALUE` 的引用。
   
2. **获取字段 ID**：使用 `GetFieldID` 获取字段 `suspensionValue` 和它包含的所有字段 (`VMCLvlTarFL`, `VMCLvlTarRR`, `CdcVMCEcasModeCHASts`, `VMCECASComplSts`) 的 ID。

3. **获取字段值**：使用 `GetFloatField` 和 `GetIntField` 分别获取 `float` 和 `int` 类型字段的值。

4. **输出值**：将获取到的字段值打印出来，供验证使用。

### 4. 注意事项

- 确保 Java 类和字段名正确。
- 在 C++ 代码中，添加对 `NULL` 的检查，防止获取不到类或字段 ID 时导致崩溃。
- 确保 JNI 方法签名与 Java 代码中的定义一致。

通过这些步骤，可以通过 JNI 成功接收并处理 `suspensionValue` 的所有字段数据。