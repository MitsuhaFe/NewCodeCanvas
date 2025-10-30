/// <reference types="vite/client" />

declare module '*.vue' {
  import type { DefineComponent } from 'vue'
  const component: DefineComponent<{}, {}, any>
  export default component
}

// WebView2 类型声明
interface Window {
  chrome?: {
    webview?: {
      postMessage(message: string): void
      addEventListener(type: string, listener: (event: any) => void): void
      removeEventListener(type: string, listener: (event: any) => void): void
    }
  }
}

