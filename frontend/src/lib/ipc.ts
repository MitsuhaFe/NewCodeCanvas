/**
 * IPC 通信客户端
 * 用于与 C++ 后端进行 JSON-RPC 2.0 通信
 */

interface JsonRpcRequest {
  jsonrpc: '2.0'
  method: string
  params?: any
  id: number
}

interface JsonRpcResponse {
  jsonrpc: '2.0'
  result?: any
  error?: {
    code: number
    message: string
    data?: any
  }
  id: number
}

interface PendingRequest {
  resolve: (result: any) => void
  reject: (error: any) => void
}

class IPCClient {
  private nextId = 1
  private pendingRequests = new Map<number, PendingRequest>()
  private eventListeners = new Map<string, Set<Function>>()

  constructor() {
    // 检查是否在 WebView2 环境中
    if (typeof window !== 'undefined' && (window as any).chrome?.webview) {
      this.initialize()
    } else {
      console.warn('Not running in WebView2 environment')
    }
  }

  private initialize() {
    // 监听来自后端的消息
    (window as any).chrome.webview.addEventListener('message', (event: any) => {
      try {
        const message = JSON.parse(event.data)
        this.handleMessage(message)
      } catch (error) {
        console.error('Failed to parse message:', error)
      }
    })

    console.log('IPC Client initialized')
  }

  private handleMessage(message: any) {
    if (message.id !== undefined) {
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
      // 通知消息（事件）
      this.handleNotification(message.method, message.params)
    }
  }

  private handleNotification(method: string, params: any) {
    const listeners = this.eventListeners.get(method)
    if (listeners) {
      listeners.forEach(callback => {
        try {
          callback(params)
        } catch (error) {
          console.error(`Error in event listener for ${method}:`, error)
        }
      })
    }
  }

  /**
   * 调用后端方法
   */
  async call<T = any>(method: string, params: any = {}): Promise<T> {
    const id = this.nextId++

    const request: JsonRpcRequest = {
      jsonrpc: '2.0',
      method,
      params,
      id
    }

    return new Promise((resolve, reject) => {
      this.pendingRequests.set(id, { resolve, reject })

      // 发送消息
      if ((window as any).chrome?.webview) {
        (window as any).chrome.webview.postMessage(JSON.stringify(request))
      } else {
        // 不在 WebView2 中，模拟响应用于开发
        setTimeout(() => {
          resolve({
            appVersion: '1.0.0-dev',
            platform: 'Development Mode'
          } as any)
        }, 100)
      }

      // 超时处理
      setTimeout(() => {
        if (this.pendingRequests.has(id)) {
          this.pendingRequests.delete(id)
          reject(new Error('Request timeout'))
        }
      }, 30000)
    })
  }

  /**
   * 订阅事件
   */
  on(event: string, callback: Function) {
    if (!this.eventListeners.has(event)) {
      this.eventListeners.set(event, new Set())
    }
    this.eventListeners.get(event)!.add(callback)
  }

  /**
   * 取消订阅事件
   */
  off(event: string, callback: Function) {
    const listeners = this.eventListeners.get(event)
    if (listeners) {
      listeners.delete(callback)
    }
  }
}

// 导出单例
export const ipc = new IPCClient()

