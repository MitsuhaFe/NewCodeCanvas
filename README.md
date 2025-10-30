# CodeCanvas - 桌面美化软件

<div align="center">

![CodeCanvas Logo](docs/assets/logo.png)

**轻量级 · 高性能 · 现代化**

一款基于 C++ 和 Vue 开发的 Windows 桌面美化软件

[功能特性](#-功能特性) · [技术架构](#-技术架构) · [快速开始](#-快速开始) · [开发文档](#-开发文档) · [贡献指南](#-贡献指南)

</div>

---

## ✨ 功能特性

### 🎯 智能 Dock 栏
- 🚀 快速启动常用应用
- 🎨 精美的图标动画效果
- 🔄 自动检测运行中应用
- 🎭 多主题支持
- ⚙️ 高度自定义（位置、大小、样式）

### 🖼️ 壁纸管理系统
- 📸 静态壁纸一键切换
- 🎬 动态视频壁纸播放
- ⏰ 定时自动切换
- 🖥️ 多显示器独立设置
- 📚 壁纸库管理与标签

### 🐱 虚拟桌宠
- 🎮 多种可爱桌宠模型
- 🏃 流畅的动画表现
- 🤝 支持拖拽交互
- 🎲 物理引擎模拟
- 💬 AI 对话能力（可选）

## 🏗️ 技术架构

### 核心技术栈

```
┌─────────────────────────────────────┐
│         前端层 (Vue 3)              │
│  • TypeScript                       │
│  • Pinia 状态管理                   │
│  • GSAP 动画引擎                    │
└───────────┬─────────────────────────┘
            │ WebView2 Bridge
┌───────────▼─────────────────────────┐
│      C++ 核心引擎                    │
│  • Win32 API                        │
│  • Direct2D / DirectX 11            │
│  • FFmpeg 视频解码                  │
│  • SQLite 数据库                    │
└──────────────────────────────────────┘
```

### 技术亮点

- ⚡ **高性能**：C++ 核心 + GPU 硬件加速
- 🪶 **轻量级**：安装包 < 50MB，运行内存 < 100MB
- 🎯 **低延迟**：60fps 流畅动画，响应时间 < 16ms
- 🔧 **模块化**：清晰的架构设计，易于扩展

## 📦 系统要求

### 运行环境
- **操作系统**：Windows 10/11 (64-bit)
- **内存**：4GB RAM（推荐 8GB+）
- **显卡**：支持 DirectX 11
- **磁盘空间**：200MB

### 开发环境
- **Visual Studio 2022** (C++ 开发)
- **Node.js 18+** (前端开发)
- **CMake 3.20+**
- **Git**

## 🚀 快速开始

### 用户安装

1. **下载安装包**
   ```
   https://github.com/your-org/codecanvas/releases/latest
   ```

2. **运行安装程序**
   ```
   CodeCanvas-1.0.0-Setup.exe
   ```

3. **首次启动**
   - 选择 Dock 栏位置
   - 添加常用应用
   - 设置喜欢的壁纸
   - 生成你的第一个桌宠

### 开发者快速开始

```bash
# 1. 克隆仓库
git clone https://github.com/your-org/codecanvas.git
cd codecanvas

# 2. 安装依赖（C++）
cd C:\vcpkg
.\vcpkg install nlohmann-json spdlog sqlite3 --triplet x64-windows

# 3. 构建后端
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

# 4. 构建前端
cd ../frontend
npm install
npm run build

# 5. 运行
cd ../bin
.\CodeCanvas.exe
```

详细的开发环境配置请参阅：[开发环境配置文档](./docs/07-开发环境配置.md)

## 📚 开发文档

完整的技术文档位于 [`docs/`](./docs/) 目录：

### 📖 核心文档

| 文档 | 说明 |
|------|------|
| [00-项目总览](./docs/00-项目总览.md) | 项目概述、目标、里程碑 |
| [01-技术选型](./docs/01-技术选型.md) | 技术栈选择与理由 |
| [02-系统架构设计](./docs/02-系统架构设计.md) | 整体架构与核心模块 |
| [03-模块设计](./docs/03-模块设计.md) | Dock/壁纸/桌宠详细设计 |
| [04-数据库设计](./docs/04-数据库设计.md) | 数据库架构与表结构 |
| [05-API设计](./docs/05-API设计.md) | 前后端通信接口 |
| [06-UI设计规范](./docs/06-UI设计规范.md) | UI/UX 设计标准 |
| [07-开发环境配置](./docs/07-开发环境配置.md) | 环境搭建指南 |
| [08-构建与部署](./docs/08-构建与部署.md) | 构建打包流程 |
| [09-性能优化指南](./docs/09-性能优化指南.md) | 性能优化技巧 |
| [10-测试方案](./docs/10-测试方案.md) | 测试策略与用例 |

👉 **从 [文档首页](./docs/README.md) 开始阅读**

## 🎨 截图预览

<div align="center">

### Dock 栏
![Dock Screenshot](docs/assets/screenshots/dock.png)

### 壁纸管理
![Wallpaper Screenshot](docs/assets/screenshots/wallpaper.png)

### 虚拟桌宠
![Pet Screenshot](docs/assets/screenshots/pet.png)

### 设置界面
![Settings Screenshot](docs/assets/screenshots/settings.png)

</div>

## 🗓️ 开发路线图

### Phase 1: 基础框架 ✅ (已完成)
- [x] 项目架构设计
- [x] 技术选型确认
- [x] 开发文档编写
- [ ] 核心框架实现

### Phase 2: Dock 栏 🚧 (进行中)
- [ ] Dock 窗口渲染
- [ ] 应用管理
- [ ] 图标系统
- [ ] 动画系统

### Phase 3: 壁纸系统 ⏳ (计划中)
- [ ] 静态壁纸引擎
- [ ] 视频壁纸引擎
- [ ] 壁纸库管理

### Phase 4: 桌宠系统 ⏳ (计划中)
- [ ] 桌宠渲染引擎
- [ ] 动画系统
- [ ] 物理引擎

### Phase 5: 优化与发布 ⏳ (计划中)
- [ ] 性能优化
- [ ] 完整测试
- [ ] 安装包制作
- [ ] 正式发布

## 🤝 贡献指南

我们欢迎各种形式的贡献！

### 如何贡献

1. **Fork 本仓库**
2. **创建特性分支** (`git checkout -b feature/AmazingFeature`)
3. **提交更改** (`git commit -m 'feat: add some amazing feature'`)
4. **推送到分支** (`git push origin feature/AmazingFeature`)
5. **开启 Pull Request**

### 贡献类型

- 🐛 **Bug 修复**：修复已知问题
- ✨ **新功能**：实现新的功能模块
- 📝 **文档改进**：完善或更新文档
- 🎨 **UI/UX 改进**：优化界面设计
- ⚡ **性能优化**：提升性能
- 🧪 **测试**：添加测试用例

### 代码规范

- **C++**：遵循 Google C++ Style Guide
- **Vue/TS**：遵循 Vue 3 官方风格指南
- **提交信息**：遵循 Conventional Commits

详情请参阅：[贡献指南](./CONTRIBUTING.md)

## 📊 项目统计

![GitHub stars](https://img.shields.io/github/stars/mitsuhafe/newcodecanvas?style=social)
![GitHub forks](https://img.shields.io/github/forks/mitsuhafe/newcodecanvas?style=social)
![GitHub issues](https://img.shields.io/github/issues/mitsuhafe/newcodecanvas)
![GitHub license](https://img.shields.io/github/license/mitsuhafe/newcodecanvas)
![Build Status](https://img.shields.io/github/actions/workflow/status/mitsuhafe/newcodecanvas/ci.yml)

## 🌟 Star 历史

[![Star History Chart](https://api.star-history.com/svg?repos=mitsuhafe/newcodecanvas&type=Date)](https://star-history.com/#mitsuhafe/newcodecanvas&Date)

## 👥 团队成员

- **项目负责人**：MitsuhaFe
- **技术负责人**：MitsuhaFe
- **后端开发**：MitsuhaFe
- **前端开发**：MitsuhaFe
- **UI/UX 设计**：MitsuhaFe

## 📄 许可证

本项目采用 [MIT License](./LICENSE) 开源协议。

```
MIT License

Copyright (c) 2025 CodeCanvas Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
```

## 🙏 致谢

感谢以下开源项目和资源：

- [Microsoft WebView2](https://github.com/MicrosoftEdge/WebView2Feedback)
- [Vue.js](https://github.com/vuejs/core)
- [FFmpeg](https://ffmpeg.org/)
- [SQLite](https://www.sqlite.org/)
- [spdlog](https://github.com/gabime/spdlog)
- [nlohmann/json](https://github.com/nlohmann/json)
- [GSAP](https://greensock.com/gsap/)

## 📞 联系我们

- **Email**：mitsuhafe@gmail.com
- **GitHub Issues**：[提交问题](https://github.com/mitsuhafe/newcodecanvas/issues)
- **讨论区**：[GitHub Discussions](https://github.com/mitsuhafe/newcodecanvas/discussions)

## ⭐ 支持我们

如果你觉得这个项目有帮助，请给我们一个 Star ⭐️

[![Star This Project](https://img.shields.io/github/stars/mitsuhafe/newcodecanvas?style=social)](https://github.com/mitsuhafe/newcodecanvas)

---

<div align="center">

**用 ❤️ 打造的桌面美化工具**

[官网](https://codecanvas.example.com) · [文档](./docs/) · [下载](https://github.com/mitsuhafe/newcodecanvas/releases) · [反馈](https://github.com/mitsuhafe/newcodecanvas/issues)

</div>

