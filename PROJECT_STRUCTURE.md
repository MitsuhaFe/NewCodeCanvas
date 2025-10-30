# CodeCanvas 项目结构

## 📁 目录结构

```
CodeCanvas/
├── backend/                    # C++ 后端
│   ├── include/               # 头文件
│   │   ├── core/             # 核心模块
│   │   │   ├── Application.h
│   │   │   └── WindowManager.h
│   │   ├── ui/               # UI 模块
│   │   │   └── MainWindow.h
│   │   └── ipc/              # 进程间通信
│   │       └── IPCManager.h
│   ├── src/                  # 源文件
│   │   ├── main.cpp
│   │   ├── core/
│   │   │   ├── Application.cpp
│   │   │   └── WindowManager.cpp
│   │   ├── ui/
│   │   │   └── MainWindow.cpp
│   │   └── ipc/
│   │       └── IPCManager.cpp
│   ├── libs/                 # 第三方库
│   │   └── WebView2/         # WebView2 SDK
│   ├── CMakeLists.txt        # CMake 构建配置
│   └── vcpkg.json           # vcpkg 依赖配置
│
├── frontend/                  # Vue 前端
│   ├── src/
│   │   ├── App.vue          # 主应用组件
│   │   ├── main.ts          # 入口文件
│   │   ├── lib/             # 工具库
│   │   │   └── ipc.ts       # IPC 客户端
│   │   ├── styles/          # 样式
│   │   │   └── global.css
│   │   └── vite-env.d.ts    # 类型声明
│   ├── index.html           # HTML 模板
│   ├── package.json         # npm 配置
│   ├── vite.config.ts       # Vite 配置
│   └── tsconfig.json        # TypeScript 配置
│
├── docs/                      # 开发文档
│   ├── README.md             # 文档导航
│   ├── 00-项目总览.md
│   ├── 01-技术选型.md
│   ├── 02-系统架构设计.md
│   ├── 03-模块设计.md
│   ├── 04-数据库设计.md
│   ├── 05-API设计.md
│   ├── 06-UI设计规范.md
│   ├── 07-开发环境配置.md
│   ├── 08-构建与部署.md
│   ├── 09-性能优化指南.md
│   └── 10-测试方案.md
│
├── scripts/                   # 构建脚本
│   ├── build-dev.ps1         # 开发构建脚本
│   ├── run-dev.ps1           # 开发运行脚本
│   └── setup-webview2.ps1    # WebView2 设置脚本
│
├── resources/                 # 资源文件
│   ├── web/                  # 前端构建输出（自动生成）
│   └── README.md
│
├── build/                     # CMake 构建目录（gitignore）
├── bin/                       # 可执行文件输出（gitignore）
│   ├── Debug/
│   │   └── CodeCanvas.exe
│   └── Release/
│       └── CodeCanvas.exe
│
├── logs/                      # 日志目录
│   └── codecanvas.log
│
├── .gitignore                # Git 忽略配置
├── README.md                 # 项目说明
├── QUICKSTART.md             # 快速开始指南
├── BUILD.md                  # 构建说明
├── PROJECT_STATUS.md         # 项目状态
└── PROJECT_STRUCTURE.md      # 本文件
```

## 📄 关键文件说明

### 后端核心文件

| 文件 | 说明 |
|------|------|
| `backend/src/main.cpp` | 应用程序入口 |
| `backend/src/core/Application.cpp` | 应用核心逻辑 |
| `backend/src/core/WindowManager.cpp` | 窗口管理 |
| `backend/src/ui/MainWindow.cpp` | 主窗口实现 |
| `backend/src/ipc/IPCManager.cpp` | IPC 通信管理 |
| `backend/CMakeLists.txt` | CMake 构建配置 |

### 前端核心文件

| 文件 | 说明 |
|------|------|
| `frontend/src/main.ts` | 前端入口 |
| `frontend/src/App.vue` | 主应用组件 |
| `frontend/src/lib/ipc.ts` | IPC 客户端 |
| `frontend/vite.config.ts` | Vite 构建配置 |
| `frontend/package.json` | npm 配置 |

### 配置文件

| 文件 | 说明 |
|------|------|
| `.gitignore` | Git 忽略规则 |
| `backend/vcpkg.json` | C++ 依赖配置 |
| `frontend/tsconfig.json` | TypeScript 配置 |
| `frontend/vite.config.ts` | Vite 配置 |

### 文档文件

| 文件 | 说明 |
|------|------|
| `README.md` | 项目主页 |
| `QUICKSTART.md` | 快速开始 |
| `BUILD.md` | 构建说明 |
| `PROJECT_STATUS.md` | 项目进度 |
| `docs/` | 详细技术文档 |

### 脚本文件

| 文件 | 说明 |
|------|------|
| `scripts/build-dev.ps1` | 开发构建 |
| `scripts/run-dev.ps1` | 开发运行 |
| `scripts/setup-webview2.ps1` | WebView2 设置 |

## 🔄 数据流

### 应用启动流程

```
WinMain()
  └─> Application::Initialize()
      ├─> InitializeLogging()
      ├─> WindowManager::Initialize()
      ├─> MainWindow::Create()
      │   └─> InitializeWebView()
      │       ├─> CreateCoreWebView2Environment()
      │       ├─> CreateCoreWebView2Controller()
      │       └─> IPCManager::Initialize()
      └─> MainWindow::Show()
  
  └─> Application::Run()
      └─> [消息循环]

  └─> Application::Shutdown()
```

### IPC 通信流程

```
Frontend (Vue)                Backend (C++)
    │                             │
    │  postMessage(request)       │
    ├──────────────────────────>  │
    │                             │
    │                     IPCManager::OnMessageReceived()
    │                             │
    │                     HandleRequest()
    │                             │
    │                     Execute Handler
    │                             │
    │  <──────────────────────────┤
    │    PostWebMessageAsJson()   │
    │                             │
   ipc.call() resolves            │
```

### 构建流程

```
源代码
  │
  ├─> Frontend Build (Vite)
  │   ├─> TypeScript 编译
  │   ├─> Vue 编译
  │   ├─> CSS 处理
  │   └─> 打包优化
  │       └─> resources/web/
  │
  └─> Backend Build (CMake)
      ├─> 生成项目文件
      ├─> C++ 编译
      ├─> 链接库
      │   ├─> WebView2
      │   ├─> nlohmann/json
      │   └─> spdlog
      └─> 输出可执行文件
          └─> bin/Debug/CodeCanvas.exe
```

## 🏗️ 架构层次

### 分层架构

```
┌─────────────────────────────────┐
│  Presentation Layer (Vue)       │  前端展示层
└────────────┬────────────────────┘
             │ WebView2
┌────────────▼────────────────────┐
│  IPC Layer (JSON-RPC)           │  通信层
└────────────┬────────────────────┘
             │
┌────────────▼────────────────────┐
│  Application Layer (C++)        │  应用层
│  - DockManager (未实现)         │
│  - WallpaperManager (未实现)    │
│  - PetManager (未实现)          │
└────────────┬────────────────────┘
             │
┌────────────▼────────────────────┐
│  Core Layer                     │  核心层
│  - WindowManager               │
│  - Application                 │
│  - IPCManager                  │
└────────────┬────────────────────┘
             │
┌────────────▼────────────────────┐
│  Platform Layer                 │  平台层
│  - Win32 API                   │
│  - WebView2                    │
│  - spdlog                      │
└──────────────────────────────────┘
```

## 🎯 模块依赖关系

```
Application
  ├─> WindowManager
  │   └─> BaseWindow
  │       └─> MainWindow
  │           ├─> WebView2
  │           └─> IPCManager
  │
  ├─> DockManager (未实现)
  ├─> WallpaperManager (未实现)
  └─> PetManager (未实现)
```

## 📦 依赖关系

### C++ 依赖

```
CodeCanvas
  ├─> Windows SDK
  │   ├─> d2d1.lib
  │   ├─> dwrite.lib
  │   └─> shlwapi.lib
  │
  ├─> WebView2 SDK
  │   └─> WebView2LoaderStatic.lib
  │
  └─> vcpkg packages
      ├─> nlohmann-json (JSON 解析)
      └─> spdlog (日志)
```

### 前端依赖

```
Frontend
  ├─> Vue 3 (框架)
  ├─> Pinia (状态管理)
  ├─> GSAP (动画)
  ├─> TypeScript (类型系统)
  └─> Vite (构建工具)
```

## 🔑 关键设计模式

### 单例模式

- `Application` - 应用程序单例
- `IPCClient` - IPC 客户端单例

### 观察者模式

- IPC 事件系统
- WebView2 消息监听

### 工厂模式

- Window 创建

### RAII 模式

- 资源管理（智能指针）
- 窗口生命周期

## 💡 开发提示

### 添加新功能

1. **后端**：在 `backend/src/` 添加新模块
2. **前端**：在 `frontend/src/` 添加新组件
3. **IPC**：在 `IPCManager` 注册新方法
4. **文档**：更新相关文档

### 调试技巧

- **后端**：使用 Visual Studio 调试器
- **前端**：使用 Chrome DevTools（F12）
- **IPC**：查看控制台日志

### 性能优化

- 使用 Release 构建
- 启用编译器优化
- 减少不必要的重绘

## 📚 相关文档

- [README.md](./README.md) - 项目主页
- [QUICKSTART.md](./QUICKSTART.md) - 快速开始
- [BUILD.md](./BUILD.md) - 构建说明
- [PROJECT_STATUS.md](./PROJECT_STATUS.md) - 项目进度
- [docs/](./docs/) - 详细技术文档

---

**更新日期**：2025-10-29

