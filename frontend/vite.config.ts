import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { resolve } from 'path'

export default defineConfig({
  plugins: [vue()],
  
  resolve: {
    alias: {
      '@': resolve(__dirname, 'src')
    }
  },
  
  base: './',
  
  build: {
    outDir: '../resources/web',
    emptyOutDir: true,
    target: 'es2020',
    minify: 'terser',
    sourcemap: false,
    rollupOptions: {
      output: {
        manualChunks: {
          'vue-vendor': ['vue', 'pinia'],
          'animation': ['gsap']
        }
      }
    }
  },
  
  server: {
    port: 5173,
    strictPort: true,
    host: '127.0.0.1'
  }
})

