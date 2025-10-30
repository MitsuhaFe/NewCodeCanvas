# CodeCanvas 开发运行脚本

$ErrorActionPreference = "Stop"

Write-Host "=== CodeCanvas 开发模式 ===" -ForegroundColor Cyan

# 检查是否已构建
if (-not (Test-Path "bin\Debug\CodeCanvas.exe")) {
    Write-Host "未找到可执行文件，正在构建..." -ForegroundColor Yellow
    .\scripts\build-dev.ps1
    if ($LASTEXITCODE -ne 0) {
        exit 1
    }
}

# 启动前端开发服务器（后台）
Write-Host "`n启动前端开发服务器..." -ForegroundColor Yellow
$frontendProcess = Start-Process powershell -ArgumentList "-NoExit", "-Command", "cd frontend; npm run dev" -PassThru

# 等待一下让服务器启动
Start-Sleep -Seconds 3

# 启动后端
Write-Host "`n启动后端应用..." -ForegroundColor Yellow
Set-Location bin\Debug
.\CodeCanvas.exe

# 清理
if ($frontendProcess -and !$frontendProcess.HasExited) {
    Write-Host "`n停止前端开发服务器..." -ForegroundColor Yellow
    Stop-Process -Id $frontendProcess.Id -Force
}

