# 下载并设置 WebView2 SDK

$ErrorActionPreference = "Stop"

Write-Host "=== WebView2 SDK 设置 ===" -ForegroundColor Cyan

$sdkVersion = "1.0.2210.55"
$nugetUrl = "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/$sdkVersion"
$sdkDir = "backend/libs/WebView2"

# 创建目录
if (-not (Test-Path $sdkDir)) {
    New-Item -ItemType Directory -Force -Path $sdkDir | Out-Null
}

# 下载 NuGet 包
Write-Host "下载 WebView2 SDK $sdkVersion..." -ForegroundColor Yellow
$tempZip = "$env:TEMP/webview2.zip"

try {
    Invoke-WebRequest -Uri $nugetUrl -OutFile $tempZip
    Write-Host "下载完成" -ForegroundColor Green
    
    # 解压
    Write-Host "解压 SDK..." -ForegroundColor Yellow
    Expand-Archive -Path $tempZip -DestinationPath $sdkDir -Force
    
    # 清理
    Remove-Item $tempZip
    
    Write-Host "`nWebView2 SDK 设置完成!" -ForegroundColor Green
    Write-Host "位置: $sdkDir"
    
} catch {
    Write-Host "错误: $_" -ForegroundColor Red
    Write-Host "`n请手动下载 WebView2 SDK:" -ForegroundColor Yellow
    Write-Host "https://www.nuget.org/packages/Microsoft.Web.WebView2/"
    exit 1
}

