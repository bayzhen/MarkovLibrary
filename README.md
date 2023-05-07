# MarkovLibrary

各种算法的实验室
```mermaid
actor 用户 as user
usecase "创建强化学习任务" as UC1
usecase "管理强化学习任务" as UC2
usecase "调整强化学习参数" as UC3
usecase "查看训练结果" as UC4
usecase "与虚幻引擎交互" as UC5
usecase "与后端交互" as UC6

user --> UC1
user --> UC2
user --> UC3
user --> UC4

actor 前端 as frontend
frontend --> UC5
frontend --> UC6

actor 虚幻引擎中间层 as ue_layer
ue_layer <-- UC5

actor 后端 as backend
backend <-- UC6
```
