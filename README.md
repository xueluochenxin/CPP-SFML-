# MY FIRST CPP PROJECT  (CPP-SFML-)

一款基于 **Modern C++ (C++14)** 与 **SFML** 引擎从零构建的 2D 弹幕射击（STG）游戏，致敬《碧蓝航线》核心战斗逻辑。

本项目旨在实践面向对象编程（OOP）、现代 C++ 内存管理规范以及游戏底层的核心架构设计，实现了完整的“Core Gameplay Loop（核心玩法循环）”。

## 💡 技术亮点与工程实现

* **现代 C++ 内存管理**：全面弃用裸指针（Raw Pointers），深度使用 `std::unique_ptr` 与 STL 容器管理实体生命周期。结合 `remove_if` 惯用法实现安全、高效的内存回收，杜绝内存泄漏。
* **多态实体架构 (Entity-Based)**：设计了统一的 `Entity` 虚拟基类，利用多态机制统一管理 Player、Enemy 与 Bullet 的 `update()` 与 `draw()` 逻辑，确保渲染与逻辑的高内聚低耦合。
* **便携式环境部署 (Portable Engineering)**：摒弃了脆弱的本地绝对路径依赖。通过 VS 宏变量 `$(SolutionDir)` 配置相对路径，并将 SFML 动态链接库（DLL）直接内置。克隆仓库后**无需繁琐的环境配置，支持一键编译运行**。
* **高性能碰撞系统**：基于 AABB（轴对齐包围盒）算法实现弹幕与实体的双向相交检测，并优化了遍历逻辑，确保在大量弹幕同屏时仍能 60FPS 满帧运行。

## 🎮 游戏特性 (Features)

* **全向机动控制**：丝滑的键盘控制手感，内置严格的屏幕边界物理碰撞检测。
* **多维武器系统**：
    * **主炮**：高频自动基础弹幕。
    * **重型鱼雷 (J键)**：单体高伤打击，内置 5.0 秒 CD 状态机逻辑。
    * **航母支援 (K键)**：全屏瞬间 AOE 真实伤害结算，通过布尔锁（Boolean Latch）确保单帧精准生效。
* **敌军 AI 管家**：由独立的 `EnemyManager` 调度，实现了多种敌舰的随机生成、边界巡航以及独立解耦的敌军火力网络。
* **完整状态机逻辑**：
    * **HUD 系统**：实时渲染得分（Score）与战斗信息。
    * **Game Over 结算**：时间冻结效果、终局分数颜色切换及遮罩弹出。
    * **重启机制 (R键)**：基于 RAII 机制实现的瞬间重置战场，无需重新加载资源。

## 🚀 快速开始 (Getting Started)

1.  **克隆本项目到本地**：
    ```bash
    git clone [https://github.com/xueluochenxin/CPP-SFML-.git](https://github.com/xueluochenxin/CPP-SFML-.git)
    ```
2.  **环境要求**：Windows 系统，安装有 Visual Studio 2022（需包含 “使用 C++ 的桌面开发” 工作负载）。
3.  **编译运行**：
    * 双击打开工程根目录的 `azur lane.sln` 解决方案文件。
    * 直接在 Visual Studio 中点击 **本地 Windows 调试器 (F5)**。
    * 项目已内置 SFML 2.6.x 库文件，无需额外配置环境变量。

---
*注：本项目仅用于学习交流及代码能力展示。*
