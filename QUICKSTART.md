# CodeCanvas 快速开始指南

本指南帮助你快速搭建 CodeCanvas 开发环境并运行第一个版本。

## 前置要求

### 必需软件

1. **Visual Studio 2022**
   - 下载：https://visualstudio.microsoft.com/downloads/
   - 组件：C++ 桌面开发工具、Windows 11 SDK

2. **Node.js 18+**
   - 下载：https://nodejs.org/
   - 推荐：LTS 版本

3. **CMake 3.20+**
   - 通常随 Visual Studio 安装
   - 或单独下载：https://cmake.org/download/

4. **Git**
   - 下载：https://git-scm.com/downloads

5. **vcpkg**（C++ 包管理器）
   - 稍后安装

## 安装步骤

### 1. 克隆项目

```powershell
git clone https://github.com/mitsuhafe/newcodecanvas.git
cd newcodecanvas
```

### 2. 安装 vcpkg

```powershell
# 克隆 vcpkg
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

# 运行 bootstrap
.\bootstrap-vcpkg.bat

# 集成到系统
.\vcpkg integrate install

# 设置环境变量
$env:VCPKG_ROOT = "C:\vcpkg"
[System.Environment]::SetEnvironmentVariable('VCPKG_ROOT', 'C:\vcpkg', 'User')
```

### 3. 安装 C++ 依赖

```powershell
cd C:\vcpkg

# 安装必需的库
.\vcpkg install nlohmann-json:x64-windows
.\vcpkg install spdlog:x64-windows

# 验证安装
.\vcpkg list
```

### 4. 设置 WebView2 SDK

```powershell
cd D:\CodeProject\PBL2\CodeCanvas  # 你的项目路径

# 运行自动设置脚本
.\scripts\setup-webview2.ps1
```

如果自动下载失败，请手动下载：
1. 访问：https://www.nuget.org/packages/Microsoft.Web.WebView2/
2. 下载最新版本
3. 解压到 `backend/libs/WebView2/`

### 5. 安装前端依赖

```powershell
cd frontend
npm install
```

## 构建和运行

### 方式一：一键构建（推荐）

```powershell
# 在项目根目录
.\scripts\build-dev.ps1

# 首次构建建议使用 -Clean
.\scripts\build-dev.ps1 -Clean
```

### 方式二：分步构建

#### 构建前端

```powershell
cd frontend
npm run build
```

#### 构建后端

```powershell
# 创建构建目录
mkdir build
cd build

# 生成项目
cmake .. -G "Visual Studio 17 2022" -A x64

# 编译
cmake --build . --config Debug

# 运行
cd ..\bin\Debug
.\CodeCanvas.exe
```

## 开发模式

### 启动开发服务器

前端支持热重载，后端连接到开发服务器：

```powershell
# 终端1：启动前端开发服务器
cd frontend
npm run dev
# 访问: http://localhost:5173

# 终端2：运行后端
cd bin\Debug
.\CodeCanvas.exe
```

### 或使用自动脚本

```powershell
.\scripts\run-dev.ps1
```

## 验证安装

运行后应该看到：

1. **控制台输出**：
```
[2025-10-29 XX:XX:XX.XXX] [info] CodeCanvas 正在启动...
[2025-10-29 XX:XX:XX.XXX] [info] 版本: 1.0.0-dev
[2025-10-29 XX:XX:XX.XXX] [info] 主窗口创建成功
[2025-10-29 XX:XX:XX.XXX] [info] WebView2 初始化成功
```

2. **应用窗口**：
   - 标题：CodeCanvas - 桌面美化软件
   - 显示欢迎界面
   - 后端连接状态：已连接

3. **点击"测试连接"按钮**：
   - 弹出系统信息
   - 显示版本和平台信息

## 常见问题

### Q: vcpkg 安装依赖失败

**A:** 设置镜像（中国大陆）：
```powershell
$env:VCPKG_DOWNLOADS="C:\vcpkg_cache"
.\vcpkg install --x-buildtrees-root=C:\vcpkg_cache
```

### Q: CMake 找不到 vcpkg

**A:** 确保设置了 VCPKG_ROOT 环境变量：
```powershell
$env:VCPKG_ROOT = "C:\vcpkg"
```

### Q: WebView2 初始化失败

**A:** 
1. 确保 Windows 10/11 已更新
2. 手动安装 WebView2 Runtime：https://developer.microsoft.com/microsoft-edge/webview2/
3. 检查 `backend/libs/WebView2/` 目录是否存在

### Q: 前端显示空白页面

**A:**
1. 检查控制台是否有错误
2. 确保前端已构建：`cd frontend && npm run build`
3. 检查 `resources/web/` 目录是否有文件
4. 尝试使用开发服务器：`npm run dev`

### Q: 编译错误：找不到头文件

**A:**
1. 确保所有 vcpkg 包已安装
2. 检查 CMakeLists.txt 中的路径
3. 清理重新构建：`.\scripts\build-dev.ps1 -Clean`

## 下一步

✅ 环境搭建完成后，建议阅读：

1. [开发文档](./docs/README.md) - 详细的技术文档
2. [系统架构设计](./docs/02-系统架构设计.md) - 了解架构
3. [模块设计](./docs/03-模块设计.md) - 开始开发功能

## 获取帮助

- 📖 [完整文档](./docs/)
- 🐛 [提交 Issue](https://github.com/mitsuhafe/newcodecanvas/issues)
- 💬 [讨论区](https://github.com/mitsuhafe/newcodecanvas/discussions)
- 📧 Email: mitsuhafe@gmail.com

---

祝开发顺利！🚀

