<template>
  <div class="app">
    <header class="app-header">
      <div class="logo">
        <svg width="32" height="32" viewBox="0 0 32 32" fill="none">
          <rect width="32" height="32" rx="8" fill="url(#gradient)" />
          <path d="M8 12L16 8L24 12V20L16 24L8 20V12Z" fill="white" fill-opacity="0.9" />
          <defs>
            <linearGradient id="gradient" x1="0" y1="0" x2="32" y2="32">
              <stop offset="0%" stop-color="#3B82F6" />
              <stop offset="100%" stop-color="#2563EB" />
            </linearGradient>
          </defs>
        </svg>
        <h1>CodeCanvas</h1>
      </div>
      <div class="system-info">
        <span v-if="systemInfo">{{ systemInfo.appVersion }}</span>
      </div>
    </header>

    <main class="app-main">
      <div class="welcome-section">
        <h2>欢迎使用 CodeCanvas</h2>
        <p class="subtitle">轻量级 · 高性能 · 现代化的桌面美化软件</p>
        
        <div class="features-grid">
          <div class="feature-card">
            <div class="feature-icon">🎯</div>
            <h3>智能 Dock 栏</h3>
            <p>快速启动常用应用，精美的图标动画效果</p>
          </div>
          
          <div class="feature-card">
            <div class="feature-icon">🖼️</div>
            <h3>壁纸管理</h3>
            <p>支持静态图片和动态视频壁纸</p>
          </div>
          
          <div class="feature-card">
            <div class="feature-icon">🐱</div>
            <h3>虚拟桌宠</h3>
            <p>可爱的桌面宠物，支持拖拽交互</p>
          </div>
        </div>

        <div class="status-section">
          <h3>当前状态</h3>
          <div class="status-grid">
            <div class="status-item">
              <span class="status-label">后端连接</span>
              <span :class="['status-value', isConnected ? 'connected' : 'disconnected']">
                {{ isConnected ? '已连接' : '未连接' }}
              </span>
            </div>
            <div class="status-item">
              <span class="status-label">系统版本</span>
              <span class="status-value">{{ systemInfo?.platform || '加载中...' }}</span>
            </div>
          </div>
        </div>

        <div class="actions">
          <button @click="testConnection" class="btn btn-primary">
            测试连接
          </button>
          <button @click="openDocs" class="btn btn-secondary">
            查看文档
          </button>
        </div>
      </div>
    </main>

    <footer class="app-footer">
      <p>用 ❤️ 打造的桌面美化工具</p>
      <p class="copyright">Copyright © 2025 CodeCanvas Team</p>
    </footer>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { ipc } from './lib/ipc'

const systemInfo = ref<any>(null)
const isConnected = ref(false)

onMounted(async () => {
  console.log('App mounted')
  
  // 测试连接
  try {
    const info = await ipc.call('system.getInfo', {})
    systemInfo.value = info
    isConnected.value = true
    console.log('System info:', info)
  } catch (error) {
    console.error('Failed to get system info:', error)
  }
})

const testConnection = async () => {
  try {
    const info = await ipc.call('system.getInfo', {})
    systemInfo.value = info
    isConnected.value = true
    alert('连接成功！\n' + JSON.stringify(info, null, 2))
  } catch (error) {
    isConnected.value = false
    alert('连接失败: ' + error)
  }
}

const openDocs = () => {
  window.open('https://github.com/mitsuhafe/newcodecanvas/tree/main/docs', '_blank')
}
</script>

<style scoped>
.app {
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  background: linear-gradient(135deg, #0D1117 0%, #161B22 100%);
  color: #F9FAFB;
}

.app-header {
  padding: 1.5rem 2rem;
  border-bottom: 1px solid #30363D;
  display: flex;
  justify-content: space-between;
  align-items: center;
  background: rgba(31, 41, 55, 0.5);
  backdrop-filter: blur(10px);
}

.logo {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.logo h1 {
  margin: 0;
  font-size: 1.5rem;
  font-weight: 600;
  background: linear-gradient(135deg, #3B82F6 0%, #60A5FA 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}

.system-info {
  color: #9CA3AF;
  font-size: 0.875rem;
}

.app-main {
  flex: 1;
  padding: 3rem 2rem;
  max-width: 1200px;
  width: 100%;
  margin: 0 auto;
}

.welcome-section {
  text-align: center;
}

.welcome-section h2 {
  font-size: 2.5rem;
  margin-bottom: 0.5rem;
  background: linear-gradient(135deg, #F9FAFB 0%, #9CA3AF 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}

.subtitle {
  font-size: 1.25rem;
  color: #9CA3AF;
  margin-bottom: 3rem;
}

.features-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 2rem;
  margin-bottom: 3rem;
}

.feature-card {
  background: rgba(31, 41, 55, 0.5);
  border: 1px solid #30363D;
  border-radius: 1rem;
  padding: 2rem;
  transition: all 0.3s ease;
}

.feature-card:hover {
  transform: translateY(-4px);
  border-color: #3B82F6;
  box-shadow: 0 8px 32px rgba(59, 130, 246, 0.2);
}

.feature-icon {
  font-size: 3rem;
  margin-bottom: 1rem;
}

.feature-card h3 {
  margin: 0 0 0.5rem 0;
  font-size: 1.25rem;
}

.feature-card p {
  margin: 0;
  color: #9CA3AF;
  font-size: 0.875rem;
}

.status-section {
  background: rgba(31, 41, 55, 0.5);
  border: 1px solid #30363D;
  border-radius: 1rem;
  padding: 2rem;
  margin-bottom: 2rem;
}

.status-section h3 {
  margin: 0 0 1.5rem 0;
  font-size: 1.25rem;
}

.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
  gap: 1.5rem;
}

.status-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1rem;
  background: rgba(17, 24, 39, 0.5);
  border-radius: 0.5rem;
}

.status-label {
  color: #9CA3AF;
  font-size: 0.875rem;
}

.status-value {
  font-weight: 600;
}

.status-value.connected {
  color: #10B981;
}

.status-value.disconnected {
  color: #EF4444;
}

.actions {
  display: flex;
  gap: 1rem;
  justify-content: center;
}

.btn {
  padding: 0.75rem 2rem;
  border: none;
  border-radius: 0.5rem;
  font-size: 1rem;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.2s ease;
}

.btn-primary {
  background: linear-gradient(135deg, #3B82F6 0%, #2563EB 100%);
  color: white;
}

.btn-primary:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 16px rgba(59, 130, 246, 0.4);
}

.btn-secondary {
  background: transparent;
  color: #F9FAFB;
  border: 1px solid #30363D;
}

.btn-secondary:hover {
  background: rgba(59, 130, 246, 0.1);
  border-color: #3B82F6;
}

.app-footer {
  padding: 2rem;
  text-align: center;
  border-top: 1px solid #30363D;
  background: rgba(31, 41, 55, 0.5);
  color: #9CA3AF;
  font-size: 0.875rem;
}

.app-footer p {
  margin: 0.25rem 0;
}

.copyright {
  color: #6B7280;
}
</style>

