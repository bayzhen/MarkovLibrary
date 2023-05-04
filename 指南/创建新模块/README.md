```mermaid
graph TD;
    A[开始] --> B[在Source目录下创建YourNewModule文件夹];
    B --> C[在YourNewModule下创建Private和Public子文件夹];
    C --> D[创建YourNewModule.Build.cs并填写代码];
    D --> E[创建YourNewModuleModule.cpp并填写代码];
    E --> F[在uproject文件中添加新模块];
    F --> G[在YourProject.Build.cs中添加YourNewModule依赖];
    G --> H[重新生成IDE解决方案];
    H --> I[完成];
```

//YourNewModule.Build.cs

```cs
using UnrealBuildTool;

public class YourNewModule: ModuleRules
{
        public YourNewModule(ReadOnlyTargetRules Target) : base(Target)
        {
	PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
        }
}
```

YourNewModuleModule.cpp

```cpp
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, YourNewModule);
```

在 YourProject.uproject 中的"Modules"加入:

```Json
{
            "Name": "YourNewModule",
            "Type": "模块类型"
}
```

YourProject.Build.cs

```cpp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "YourNewModule" });
```
