# CodeCanvas 构建说明

## 📋 目录

- [快速构建](#快速构建)
- [详细步骤](#详细步骤)
- [常见问题](#常见问题)
- [开发模式](#开发模式)

## 🚀 快速构建

如果你已经配置好开发环境，可以使用一键构建：

```powershell
# 完整构建（首次）
.\scripts\build-dev.ps1 -Clean

# 增量构建
.\scripts\build-dev.ps1
```

**首次使用？** 请先阅读 [QUICKSTART.md](./QUICKSTART.md) 配置开发环境。

## 📝 详细步骤

### 前置要求

确保已安装：
- [x] Visual Studio 2022
- [x] Node.js 18+
- [x] CMake 3.20+
- [x] vcpkg
- [x] WebView2 SDK

详见：[开发环境配置](./docs/07-开发环境配置.md)

### 步骤 1：安装依赖

#### C++ 依赖（vcpkg）

```powershell
cd C:\vcpkg

# 安装必需的库
.\vcpkg install nlohmann-json:x64-windows
.\vcpkg install spdlog:x64-windows
```

#### 前端依赖

```powershell
cd frontend
npm install
```

#### WebView2 SDK

```powershell
# 自动下载
.\scripts\setup-webview2.ps1

# 或手动下载
# https://www.nuget.org/packages/Microsoft.Web.WebView2/
# 解压到 backend/libs/WebView2/
```

### 步骤 2：构建前端

```powershell
cd frontend

# 开发构建（生成到 resources/web/）
npm run build

# 或启动开发服务器
npm run dev  # http://localhost:5173
```

### 步骤 3：构建后端

```powershell
# 返回项目根目录
cd ..

# 创建构建目录
mkdir build
cd build

# 生成 CMake 项目
cmake .. -G "Visual Studio 17 2022" -A x64 `
    -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

# 编译 Debug 版本
cmake --build . --config Debug

# 或编译 Release 版本
cmake --build . --config Release
```

### 步骤 4：运行

```powershell
# Debug 版本
cd bin\Debug
.\CodeCanvas.exe

# Release 版本
cd bin\Release
.\CodeCanvas.exe
```

## 🛠️ 构建选项

### CMake 配置选项

```powershell
# 指定 vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"

# 指定构建类型
cmake .. -DCMAKE_BUILD_TYPE=Release

# 指定生成器
cmake .. -G "Visual Studio 17 2022" -A x64
cmake .. -G "Ninja"
```

### 前端构建选项

```powershell
# 开发构建（带 sourcemap）
npm run build

# 生产构建（优化）
npm run build -- --mode production

# 预览构建结果
npm run preview
```

## 🔧 开发模式

### 方式一：前后端分离开发

**终端 1** - 前端开发服务器（热重载）：
```powershell
cd frontend
npm run dev
```

**终端 2** - 后端应用：
```powershell
cd bin\Debug
.\CodeCanvas.exe
```

后端会自动连接到 `http://localhost:5173`

### 方式二：集成开发模式

使用自动化脚本：
```powershell
.\scripts\run-dev.ps1
```

此脚本会：
1. 启动前端开发服务器（后台）
2. 启动后端应用
3. 退出时自动清理

### 方式三：完整构建开发

```powershell
# 构建前端到 resources/web/
cd frontend
npm run build

# 运行后端（加载本地文件）
cd ..\bin\Debug
.\CodeCanvas.exe
```

## 📦 打包发布

```powershell
# 完整的发布构建
.\scripts\build-release.ps1 -Version "1.0.0" -Clean

# 输出：
# - dist/CodeCanvas-1.0.0-Setup.exe (安装包)
# - dist/CodeCanvas-1.0.0-Portable.zip (绿色版)
```

详见：[构建与部署](./docs/08-构建与部署.md)

## ❓ 常见问题

### 构建失败

#### CMake 错误：找不到 vcpkg

```powershell
# 设置环境变量
$env:VCPKG_ROOT = "C:\vcpkg"

# 或在命令中指定
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

#### 链接错误：找不到 WebView2

```powershell
# 确保 WebView2 SDK 已安装
.\scripts\setup-webview2.ps1

# 检查目录
Test-Path backend/libs/WebView2/build/native/include
Test-Path backend/libs/WebView2/build/native/x64/WebView2LoaderStatic.lib
```

#### 编译错误：找不到头文件

```powershell
# 重新安装 vcpkg 依赖
cd C:\vcpkg
.\vcpkg remove nlohmann-json:x64-windows
.\vcpkg remove spdlog:x64-windows
.\vcpkg install nlohmann-json:x64-windows
.\vcpkg install spdlog:x64-windows

# 清理重新构建
.\scripts\build-dev.ps1 -Clean
```

### 运行失败

#### WebView2 初始化失败

**原因**：系统缺少 WebView2 Runtime

**解决**：
1. 下载安装：https://developer.microsoft.com/microsoft-edge/webview2/
2. 或更新 Windows 10/11 到最新版本

#### 前端显示空白

**检查**：
```powershell
# 1. 检查资源文件
Test-Path resources/web/index.html
Get-ChildItem resources/web/

# 2. 查看控制台日志
# 应该看到：[info] 加载前端页面: ...

# 3. 重新构建前端
cd frontend
Remove-Item -Recurse -Force ../resources/web
npm run build
```

#### 应用崩溃

**调试步骤**：

1. **使用 Debug 版本**：
```powershell
cd build
cmake --build . --config Debug
cd ..\bin\Debug
.\CodeCanvas.exe
```

2. **查看日志**：
```powershell
Get-Content logs/codecanvas.log -Tail 50
```

3. **Visual Studio 调试**：
   - 打开 `build/CodeCanvas.sln`
   - 设置断点
   - F5 启动调试

### 性能问题

#### 启动慢

```powershell
# 使用 Release 构建
cd build
cmake --build . --config Release
```

#### 内存占用高

```powershell
# 检查是否有内存泄漏
# Debug 模式会自动检测
cd bin\Debug
.\CodeCanvas.exe
```

## 🔍 验证构建

### 检查清单

- [ ] 编译无错误
- [ ] 链接无错误
- [ ] 可执行文件存在
- [ ] 应用可以启动
- [ ] 窗口正常显示
- [ ] 前端页面加载成功
- [ ] 后端连接状态：已连接
- [ ] 点击"测试连接"成功

### 测试命令

```powershell
# 检查可执行文件
Test-Path bin/Debug/CodeCanvas.exe

# 检查前端资源
Test-Path resources/web/index.html

# 运行并查看日志
cd bin/Debug
.\CodeCanvas.exe
# 观察控制台输出
```

## 📚 相关文档

- [快速开始指南](./QUICKSTART.md) - 新手必读
- [开发环境配置](./docs/07-开发环境配置.md) - 详细配置
- [构建与部署](./docs/08-构建与部署.md) - 高级构建
- [项目状态](./PROJECT_STATUS.md) - 当前进度

## 💡 提示

### 加速构建

```powershell
# 使用多核编译
cmake --build . --config Debug --parallel 8

# 只编译后端（跳过前端）
.\scripts\build-dev.ps1 -SkipFrontend
```

### 清理构建

```powershell
# 清理后端
Remove-Item -Recurse -Force build, bin

# 清理前端
cd frontend
Remove-Item -Recurse -Force node_modules, dist
cd ..
Remove-Item -Recurse -Force resources/web

# 完全清理
.\scripts\build-dev.ps1 -Clean
```

## 🆘 获取帮助

遇到问题？

1. **查看文档**：[docs/](./docs/)
2. **搜索 Issues**：[GitHub Issues](https://github.com/mitsuhafe/newcodecanvas/issues)
3. **提问**：[GitHub Discussions](https://github.com/mitsuhafe/newcodecanvas/discussions)
4. **联系作者**：mitsuhafe@gmail.com

---

**祝构建成功！** 🎉

