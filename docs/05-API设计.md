# API 设计文档

## API 概述

CodeCanvas 使用 **JSON-RPC 2.0** 协议进行前后端通信，通过 WebView2 的消息机制实现。

### 通信架构

```
┌──────────────────┐                    ┌──────────────────┐
│   Frontend       │                    │    Backend       │
│   (Vue 3)        │◄──────────────────►│    (C++)         │
│                  │   WebView2 Bridge  │                  │
└──────────────────┘                    └──────────────────┘
        │                                        │
        │  window.chrome.webview.postMessage()  │
        ├──────────────────────────────────────►│
        │                                        │
        │  PostWebMessageAsJson()                │
        │◄───────────────────────────────────────┤
```

## JSON-RPC 2.0 协议

### 请求格式

```typescript
interface JsonRpcRequest {
  jsonrpc: '2.0'
  method: string
  params?: object | array
  id?: number | string
}
```

**示例**：
```json
{
  "jsonrpc": "2.0",
  "method": "dock.addApp",
  "params": {
    "path": "C:\\Program Files\\Example\\app.exe"
  },
  "id": 1
}
```

### 响应格式

**成功响应**：
```typescript
interface JsonRpcSuccess {
  jsonrpc: '2.0'
  result: any
  id: number | string
}
```

```json
{
  "jsonrpc": "2.0",
  "result": {
    "id": "app-123",
    "name": "Example App",
    "path": "C:\\Program Files\\Example\\app.exe",
    "position": 0
  },
  "id": 1
}
```

**错误响应**：
```typescript
interface JsonRpcError {
  jsonrpc: '2.0'
  error: {
    code: number
    message: string
    data?: any
  }
  id: number | string | null
}
```

```json
{
  "jsonrpc": "2.0",
  "error": {
    "code": -32600,
    "message": "Invalid Request",
    "data": {
      "details": "Missing required parameter: path"
    }
  },
  "id": 1
}
```

### 通知（单向消息）

```json
{
  "jsonrpc": "2.0",
  "method": "wallpaper.changed",
  "params": {
    "id": "wallpaper-123",
    "name": "Mountain.jpg"
  }
}
```

## 错误码规范

| 错误码 | 含义 | 说明 |
|--------|------|------|
| -32700 | Parse error | JSON 解析错误 |
| -32600 | Invalid Request | 无效的请求对象 |
| -32601 | Method not found | 方法不存在 |
| -32602 | Invalid params | 无效的参数 |
| -32603 | Internal error | 内部错误 |
| -32000 to -32099 | Server error | 服务器自定义错误 |
| 1000-1999 | Business error | 业务逻辑错误 |

**业务错误码定义**：
```cpp
enum class ApiErrorCode {
  // 通用错误 (1000-1099)
  FileNotFound = 1000,
  PermissionDenied = 1001,
  ResourceExists = 1002,
  ResourceNotFound = 1003,
  
  // Dock 错误 (1100-1199)
  DockAppNotFound = 1100,
  DockAppExists = 1101,
  DockLaunchFailed = 1102,
  
  // 壁纸错误 (1200-1299)
  WallpaperLoadFailed = 1200,
  WallpaperUnsupportedFormat = 1201,
  WallpaperDecodeFailed = 1202,
  
  // 桌宠错误 (1300-1399)
  PetModelNotFound = 1300,
  PetSpawnFailed = 1301
};
```

## API 端点

### 1. Dock API

#### 1.1 dock.getApps

获取所有 Dock 应用列表。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "dock.getApps",
  "params": {},
  "id": 1
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "apps": [
      {
        "id": "app-1",
        "name": "Visual Studio Code",
        "path": "C:\\Users\\...\\Code.exe",
        "iconPath": null,
        "position": 0,
        "isPinned": true,
        "isRunning": false,
        "launchCount": 42
      }
    ]
  },
  "id": 1
}
```

#### 1.2 dock.addApp

添加应用到 Dock。

**请求**：
```typescript
interface AddAppParams {
  path: string          // 可执行文件路径
  name?: string         // 自定义名称（可选）
  iconPath?: string     // 自定义图标（可选）
  position?: number     // 插入位置（可选）
}
```

```json
{
  "jsonrpc": "2.0",
  "method": "dock.addApp",
  "params": {
    "path": "C:\\Program Files\\App\\app.exe",
    "position": 0
  },
  "id": 2
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "id": "app-123",
    "name": "App",
    "path": "C:\\Program Files\\App\\app.exe",
    "position": 0,
    "isPinned": true
  },
  "id": 2
}
```

**错误**：
```json
{
  "jsonrpc": "2.0",
  "error": {
    "code": 1101,
    "message": "App already exists in Dock",
    "data": {
      "existingId": "app-456"
    }
  },
  "id": 2
}
```

#### 1.3 dock.removeApp

从 Dock 移除应用。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "dock.removeApp",
  "params": {
    "id": "app-123"
  },
  "id": 3
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "success": true
  },
  "id": 3
}
```

#### 1.4 dock.launchApp

启动应用。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "dock.launchApp",
  "params": {
    "id": "app-123"
  },
  "id": 4
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "success": true,
    "processId": 12345
  },
  "id": 4
}
```

#### 1.5 dock.updateConfig

更新 Dock 配置。

**请求**：
```typescript
interface DockConfig {
  position?: 'top' | 'bottom' | 'left' | 'right'
  autoHide?: boolean
  iconSize?: number
  theme?: string
}
```

```json
{
  "jsonrpc": "2.0",
  "method": "dock.updateConfig",
  "params": {
    "position": "bottom",
    "autoHide": true,
    "iconSize": 64
  },
  "id": 5
}
```

#### 1.6 dock.getConfig

获取 Dock 配置。

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "position": "bottom",
    "autoHide": false,
    "iconSize": 48,
    "maxIconSize": 64,
    "theme": "default"
  },
  "id": 6
}
```

### 2. Wallpaper API

#### 2.1 wallpaper.getAll

获取所有壁纸。

**请求**：
```typescript
interface GetWallpapersParams {
  type?: 'static' | 'video' | 'web'  // 过滤类型
  tags?: string[]                     // 过滤标签
  limit?: number                      // 限制数量
  offset?: number                     // 分页偏移
  sortBy?: 'created' | 'used' | 'name'
  sortOrder?: 'asc' | 'desc'
}
```

```json
{
  "jsonrpc": "2.0",
  "method": "wallpaper.getAll",
  "params": {
    "type": "video",
    "limit": 20,
    "offset": 0
  },
  "id": 10
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "wallpapers": [
      {
        "id": "wp-1",
        "name": "Ocean Waves",
        "type": "video",
        "filePath": "D:\\Wallpapers\\ocean.mp4",
        "thumbnailPath": "D:\\Wallpapers\\thumbnails\\ocean.jpg",
        "width": 1920,
        "height": 1080,
        "duration": 120.5,
        "fps": 30,
        "fileSize": 52428800,
        "tags": ["nature", "ocean"],
        "isFavorite": true,
        "useCount": 15,
        "createdAt": 1698768000
      }
    ],
    "total": 150
  },
  "id": 10
}
```

#### 2.2 wallpaper.add

添加壁纸。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "wallpaper.add",
  "params": {
    "path": "D:\\Images\\mountain.jpg",
    "tags": ["nature", "mountain"]
  },
  "id": 11
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "id": "wp-123",
    "name": "mountain.jpg",
    "type": "static",
    "thumbnailPath": "...",
    "createdAt": 1698768000
  },
  "id": 11
}
```

#### 2.3 wallpaper.setActive

设置活动壁纸。

**请求**：
```typescript
interface SetActiveParams {
  id: string
  monitorId?: string  // 可选：特定显示器
}
```

```json
{
  "jsonrpc": "2.0",
  "method": "wallpaper.setActive",
  "params": {
    "id": "wp-123"
  },
  "id": 12
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "success": true
  },
  "id": 12
}
```

#### 2.4 wallpaper.remove

删除壁纸。

**请求**：
```typescript
interface RemoveParams {
  id: string
  deleteFile?: boolean  // 是否删除源文件
}
```

#### 2.5 wallpaper.updateTags

更新壁纸标签。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "wallpaper.updateTags",
  "params": {
    "id": "wp-123",
    "tags": ["nature", "sunset", "beach"]
  },
  "id": 13
}
```

#### 2.6 wallpaper.getMonitors

获取显示器列表。

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "monitors": [
      {
        "id": "monitor-1",
        "name": "DELL U2720Q",
        "width": 3840,
        "height": 2160,
        "isPrimary": true,
        "dpi": 163,
        "currentWallpaper": "wp-123"
      }
    ]
  },
  "id": 14
}
```

#### 2.7 wallpaper.enableAutoChange

启用自动切换。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "wallpaper.enableAutoChange",
  "params": {
    "interval": 3600,
    "playlistId": "playlist-1"
  },
  "id": 15
}
```

### 3. Pet API

#### 3.1 pet.getModels

获取可用桌宠模型。

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "models": [
      {
        "name": "cat",
        "displayName": "小猫",
        "description": "可爱的小猫咪",
        "thumbnailPath": "...",
        "animations": ["idle", "walk", "sit", "sleep"],
        "spawnCount": 42
      }
    ]
  },
  "id": 20
}
```

#### 3.2 pet.spawn

生成桌宠实例。

**请求**：
```typescript
interface SpawnParams {
  modelName: string
  x?: number         // 初始 X 坐标
  y?: number         // 初始 Y 坐标
  scale?: number     // 缩放比例
}
```

```json
{
  "jsonrpc": "2.0",
  "method": "pet.spawn",
  "params": {
    "modelName": "cat",
    "x": 100,
    "y": 100,
    "scale": 1.5
  },
  "id": 21
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "id": "pet-123",
    "modelName": "cat",
    "x": 100,
    "y": 100,
    "scale": 1.5,
    "currentAnimation": "idle"
  },
  "id": 21
}
```

#### 3.3 pet.destroy

销毁桌宠。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "pet.destroy",
  "params": {
    "id": "pet-123"
  },
  "id": 22
}
```

#### 3.4 pet.playAnimation

播放动画。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "pet.playAnimation",
  "params": {
    "id": "pet-123",
    "animation": "walk"
  },
  "id": 23
}
```

#### 3.5 pet.setScale

设置缩放。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "pet.setScale",
  "params": {
    "id": "pet-123",
    "scale": 2.0
  },
  "id": 24
}
```

#### 3.6 pet.getInstances

获取所有活动桌宠。

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "instances": [
      {
        "id": "pet-123",
        "modelName": "cat",
        "x": 500,
        "y": 800,
        "scale": 1.5,
        "currentAnimation": "idle"
      }
    ]
  },
  "id": 25
}
```

### 4. System API

#### 4.1 system.getInfo

获取系统信息。

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "appVersion": "1.0.0",
    "platform": "Windows",
    "osVersion": "10.0.22621",
    "cpuUsage": 2.5,
    "memoryUsage": 85,
    "memoryTotal": 16384
  },
  "id": 30
}
```

#### 4.2 system.openSettings

打开设置窗口。

**请求**：
```json
{
  "jsonrpc": "2.0",
  "method": "system.openSettings",
  "params": {
    "tab": "dock"
  },
  "id": 31
}
```

#### 4.3 system.checkUpdates

检查更新。

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "hasUpdate": true,
    "latestVersion": "1.1.0",
    "releaseNotes": "...",
    "downloadUrl": "https://..."
  },
  "id": 32
}
```

#### 4.4 system.selectFile

打开文件选择对话框。

**请求**：
```typescript
interface SelectFileParams {
  filters?: {
    name: string
    extensions: string[]
  }[]
  multiSelect?: boolean
}
```

```json
{
  "jsonrpc": "2.0",
  "method": "system.selectFile",
  "params": {
    "filters": [
      {
        "name": "Images",
        "extensions": ["jpg", "png", "gif"]
      },
      {
        "name": "Videos",
        "extensions": ["mp4", "webm"]
      }
    ],
    "multiSelect": true
  },
  "id": 33
}
```

**响应**：
```json
{
  "jsonrpc": "2.0",
  "result": {
    "files": [
      "D:\\Images\\photo1.jpg",
      "D:\\Images\\photo2.jpg"
    ]
  },
  "id": 33
}
```

### 5. 事件通知（Backend -> Frontend）

#### 5.1 dock.appLaunched

应用启动事件。

```json
{
  "jsonrpc": "2.0",
  "method": "dock.appLaunched",
  "params": {
    "id": "app-123",
    "name": "App",
    "timestamp": 1698768000
  }
}
```

#### 5.2 wallpaper.changed

壁纸切换事件。

```json
{
  "jsonrpc": "2.0",
  "method": "wallpaper.changed",
  "params": {
    "id": "wp-123",
    "name": "Mountain.jpg",
    "monitorId": "monitor-1"
  }
}
```

#### 5.3 pet.animationComplete

桌宠动画完成事件。

```json
{
  "jsonrpc": "2.0",
  "method": "pet.animationComplete",
  "params": {
    "petId": "pet-123",
    "animation": "walk"
  }
}
```

#### 5.4 system.error

系统错误事件。

```json
{
  "jsonrpc": "2.0",
  "method": "system.error",
  "params": {
    "code": 1200,
    "message": "Failed to load wallpaper",
    "details": "..."
  }
}
```

## 前端 API 封装（TypeScript）

### IPC 客户端

```typescript
// frontend/src/lib/ipc.ts
class IPCClient {
  private nextId = 1
  private pendingRequests = new Map<number, {
    resolve: (result: any) => void
    reject: (error: any) => void
  }>()
  
  constructor() {
    // 监听后端消息
    window.chrome.webview.addEventListener('message', (event) => {
      const message = JSON.parse(event.data)
      
      if (message.id) {
        // 响应消息
        const pending = this.pendingRequests.get(message.id)
        if (pending) {
          if (message.error) {
            pending.reject(message.error)
          } else {
            pending.resolve(message.result)
          }
          this.pendingRequests.delete(message.id)
        }
      } else if (message.method) {
        // 通知消息
        this.handleNotification(message.method, message.params)
      }
    })
  }
  
  // 调用后端方法
  async call<T = any>(method: string, params: object = {}): Promise<T> {
    const id = this.nextId++
    
    const request = {
      jsonrpc: '2.0',
      method,
      params,
      id
    }
    
    return new Promise((resolve, reject) => {
      this.pendingRequests.set(id, { resolve, reject })
      
      window.chrome.webview.postMessage(JSON.stringify(request))
      
      // 超时处理
      setTimeout(() => {
        if (this.pendingRequests.has(id)) {
          this.pendingRequests.delete(id)
          reject(new Error('Request timeout'))
        }
      }, 30000)  // 30 秒超时
    })
  }
  
  // 事件监听
  private eventListeners = new Map<string, Set<Function>>()
  
  on(event: string, callback: Function) {
    if (!this.eventListeners.has(event)) {
      this.eventListeners.set(event, new Set())
    }
    this.eventListeners.get(event)!.add(callback)
  }
  
  off(event: string, callback: Function) {
    this.eventListeners.get(event)?.delete(callback)
  }
  
  private handleNotification(method: string, params: any) {
    const listeners = this.eventListeners.get(method)
    if (listeners) {
      listeners.forEach(callback => callback(params))
    }
  }
}

export const ipc = new IPCClient()
```

### API 模块

```typescript
// frontend/src/api/index.ts
export { dockApi } from './dock'
export { wallpaperApi } from './wallpaper'
export { petApi } from './pet'
export { systemApi } from './system'

// frontend/src/api/dock.ts
import { ipc } from '@/lib/ipc'

export const dockApi = {
  async getApps() {
    const result = await ipc.call('dock.getApps', {})
    return result.apps
  },
  
  async addApp(path: string) {
    return await ipc.call('dock.addApp', { path })
  },
  
  async removeApp(id: string) {
    await ipc.call('dock.removeApp', { id })
  },
  
  async launchApp(id: string) {
    await ipc.call('dock.launchApp', { id })
  },
  
  onAppLaunched(callback: (data: any) => void) {
    ipc.on('dock.appLaunched', callback)
  }
}
```

---

*API 设计将根据前端需求持续演进和优化*

