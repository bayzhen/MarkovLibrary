# MarkovLibrary

各种算法的实验室
```mermaid
graph TD
A(开始) --> B[创建游戏环境]
B --> C[实现IEnvironment接口]
C --> D[注册游戏环境]
D --> E[与Python端建立连接]
E --> F{等待指令}
F -->|创建| G[创建强化学习环境]
G --> F
F -->|开始| H[开始训练]
H --> F
F -->|停止| I[停止训练]
I --> F
F -->|删除| J[删除强化学习环境]
J --> F
F -->|断开连接| K(结束)
```
