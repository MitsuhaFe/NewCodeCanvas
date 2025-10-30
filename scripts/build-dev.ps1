# CodeCanvas 开发构建脚本
param(
    [switch]$Clean,
    [switch]$SkipFrontend
)

$ErrorActionPreference = "Stop"

Write-Host "=== CodeCanvas 开发构建 ===" -ForegroundColor Cyan
Write-Host "工作目录: $PWD"

# 检查 vcpkg
if (-not $env:VCPKG_ROOT) {
    Write-Host "警告: VCPKG_ROOT 环境变量未设置" -ForegroundColor Yellow
    Write-Host "尝试使用默认路径: C:\vcpkg"
    $env:VCPKG_ROOT = "C:\vcpkg"
}

if (-not (Test-Path $env:VCPKG_ROOT)) {
    Write-Host "错误: vcpkg 未找到，请先安装 vcpkg" -ForegroundColor Red
    Write-Host "安装方法: docs/07-开发环境配置.md"
    exit 1
}

# 1. 清理（可选）
if ($Clean) {
    Write-Host "`n[1/4] 清理旧构建..." -ForegroundColor Yellow
    if (Test-Path "build") {
        Remove-Item -Recurse -Force build
    }
    if (Test-Path "bin") {
        Remove-Item -Recurse -Force bin
    }
    if (Test-Path "resources/web") {
        Remove-Item -Recurse -Force resources/web
    }
}

# 2. 构建前端
if (-not $SkipFrontend) {
    Write-Host "`n[2/4] 构建前端..." -ForegroundColor Yellow
    
    Set-Location frontend
    
    if (-not (Test-Path "node_modules")) {
        Write-Host "安装前端依赖..."
        npm install
        if ($LASTEXITCODE -ne 0) {
            Write-Host "前端依赖安装失败" -ForegroundColor Red
            exit 1
        }
    }
    
    Write-Host "构建前端..."
    npm run build
    if ($LASTEXITCODE -ne 0) {
        Write-Host "前端构建失败" -ForegroundColor Red
        exit 1
    }
    
    Set-Location ..
} else {
    Write-Host "`n[2/4] 跳过前端构建" -ForegroundColor Yellow
}

# 3. 构建后端
Write-Host "`n[3/4] 构建后端..." -ForegroundColor Yellow

# 创建构建目录
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Force -Path build | Out-Null
}

Set-Location build

# 生成 CMake 项目
Write-Host "生成 CMake 项目..."
cmake .. -G "Visual Studio 17 2022" -A x64 `
    -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake 生成失败" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# 编译
Write-Host "编译 C++ 代码..."
cmake --build . --config Debug --parallel 8

if ($LASTEXITCODE -ne 0) {
    Write-Host "编译失败" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Set-Location ..

# 4. 完成
Write-Host "`n[4/4] 构建完成!" -ForegroundColor Green
Write-Host ""
Write-Host "可执行文件: bin\Debug\CodeCanvas.exe" -ForegroundColor Cyan
Write-Host ""
Write-Host "运行命令:"
Write-Host "  cd bin\Debug" -ForegroundColor Yellow
Write-Host "  .\CodeCanvas.exe" -ForegroundColor Yellow
Write-Host ""
Write-Host "或者启动开发服务器（前端热重载）:"
Write-Host "  cd frontend" -ForegroundColor Yellow
Write-Host "  npm run dev" -ForegroundColor Yellow

