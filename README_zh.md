# 数据分级保护

- [数据分级保护](#数据分级保护)
  - [简介](#简介)
  - [系统架构](#系统架构)
  - [目录](#目录)
  - [约束](#约束)
  - [说明](#说明)
    - [接口说明](#接口说明)
    - [使用说明](#使用说明)
  - [相关仓](#相关仓)

## 简介

在OpenHarmony中，数据分级保护模块负责提供数据分级的保护策略。数据分级保护模块提供了数据分级相关的接口定义。（OpenHarmony当前不提供实际的功能实现。依赖设备厂商实现接口对应的功能，对搭载OpenHarmony的设备上的数据提供安全保护）。

数据分级保护模块当前提供如下接口定义：

- 基于设备安全等级的数据跨设备访问控制接口：提供基于设备安全等级的数据跨设备访问控制的接口，分布式跨设备数据传输业务可使用该接口获得对端设备可支持的数据风险等级。

## 系统架构



## 目录



```
base/security/dataclassification
├── frameworks                        
│   └── datatransmitmgr             # 代码实现
├── interfaces                        # 接口API代码
│   └── innerkits
│       └── datatransmitmgr
└── text                              #测试代码
    └── unittest
        └── datatransmitmgr
```



## 约束

- 开发语言：C

## 说明

### 接口说明

数据分级保护提供的API接口功能介绍

| 接口名                                                       | 描述                                                   |
| ------------------------------------------------------------ | :----------------------------------------------------- |
| int32_t DATASL_GetHighestSecLevel(DEVSLQueryParams *queryParams, uint32_t *levelInfo); | 请求获取对应设备允许传输的最高数据安全等级（同步接口） |
| int32_t DATASL_GetHighestSecLevelAsync(DEVSLQueryParams *queryParams, HigestSecInfoCallback *callback); | 请求获取对应设备允许传输的最高数据安全等级（异步接口） |
| int32_t DATASL_OnStart(void);                                | 初始化模块                                             |
| void DATASL_OnStop(void);                                    | 模块退出                                               |

### 使用说明

所有接口均为native C内部接口

#### 依赖添加

1、编译依赖添加

```
external_deps += [ "dataclassification:data_transit_mgr" ]
```

2、头文件添加

```
#include "dev_slinfo_mgr.h"
```

接口使用示例

1、同步接口使用示例如下：

```c++
void CheckDestHighestSecurityLevel(DEVSLQueryParams *queryParams)
{
    // 数据风险等级
    uint32_t levelInfo = 0;
    
    //初始化模块
    int32_t ret = DATASL_OnStart();
    if (ret != DEVSL_SUCCESS) {
        // 初始化模块失败。此场景建议开发者根据实际情况进行重试
        return;
    }
    
    // 调用同步接口获取设备允许的最高数据风险等级
    int32_t ret = DATASL_GetHighestSecLevel(queryParams, &levelInfo);
    if (ret != DEVSL_SUCCESS) {
        // 获取信息失败。此场景建议开发者根据实际情况进行重试
        return;
    }
    
    // 模块退出
    DATASL_OnStop();    
}
```

2、异步接口使用示例如下：

```c++
// 回调函数，callback返回错误码以及设备允许的最高数据风险等级
void HigestSecInfoCallback(DEVSLQueryParams *queryParams, int32_t result, uint32_t levelInfo)
{
    if (result != DEVSL_SUCCESS) {
        // 异步获取信息失败。此场景建议开发者根据实际情况进行重试
        return;
    }
}
void CheckDestHighestSecurityLevel(DEVSLQueryParams *queryParams)
{
    //初始化模块，重复初始化不影响功能，建议避免重复操作
    int32_t ret = DATASL_OnStart();
    
    //调用异步接口获取设备允许的最高数据风险等级
    ret = DATASL_GetHighestSecLevelAsync(queryParams, HigestSecInfoCallback);
    if (ret != DEVSL_SUCCESS) {
        // 获取信息失败，此场景建议开发者根据实际情况进行重试
        // 此场景下callback不会回调。
        return;
    }
    // 调用成功，等待callback回调。
    
    //关闭模块，注意在多线程的情况下慎重使用，当前线程关闭之后会对其他线程造成影响
    DATASL_OnStop();
}
```



## 相关仓

安全子系统

base/security/dataclassification