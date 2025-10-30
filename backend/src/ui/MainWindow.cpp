#include "ui/MainWindow.h"
#include <spdlog/spdlog.h>
#include <WebView2.h>
#include <wil/com.h>
#include <filesystem>

namespace CodeCanvas {

MainWindow::MainWindow(WindowManager& windowManager)
    : m_windowManager(windowManager) {
}

MainWindow::~MainWindow() {
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
    }
}

bool MainWindow::Create() {
    // 注册窗口类
    const wchar_t* className = L"CodeCanvasMainWindow";
    
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowManager::WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = className;
    
    if (!RegisterClassExW(&wc)) {
        DWORD error = GetLastError();
        if (error != ERROR_CLASS_ALREADY_EXISTS) {
            spdlog::error("注册窗口类失败: {}", error);
            return false;
        }
    }
    
    // 创建窗口
    m_hwnd = CreateWindowExW(
        0,
        className,
        L"CodeCanvas - 桌面美化软件",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 800,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        this  // 传递this指针
    );
    
    if (!m_hwnd) {
        spdlog::error("创建窗口失败: {}", GetLastError());
        return false;
    }
    
    m_windowManager.RegisterWindow(m_hwnd, this);
    
    // 初始化 WebView2
    InitializeWebView();
    
    spdlog::info("主窗口创建成功");
    return true;
}

void MainWindow::Show() {
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
}

void MainWindow::Hide() {
    ShowWindow(m_hwnd, SW_HIDE);
}

LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_SIZE:
            OnSize();
            return 0;
            
        case WM_DESTROY:
            OnDestroy();
            return 0;
            
        case WM_CLOSE:
            DestroyWindow(m_hwnd);
            return 0;
    }
    
    return DefWindowProc(m_hwnd, msg, wParam, lParam);
}

void MainWindow::InitializeWebView() {
    spdlog::info("初始化 WebView2...");
    
    // 创建 WebView2 环境
    auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
    
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr,
        nullptr,
        options.Get(),
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result)) {
                    spdlog::error("创建 WebView2 环境失败: {}", result);
                    return result;
                }
                
                // 创建 WebView2 控制器
                env->CreateCoreWebView2Controller(m_hwnd,
                    Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result)) {
                                spdlog::error("创建 WebView2 控制器失败: {}", result);
                                return result;
                            }
                            
                            m_webViewController = controller;
                            m_webViewController->get_CoreWebView2(&m_webView);
                            
                            // 设置 WebView 大小
                            RECT bounds;
                            GetClientRect(m_hwnd, &bounds);
                            m_webViewController->put_Bounds(bounds);
                            
                            // 创建 IPC 管理器
                            m_ipcManager = std::make_unique<IPCManager>(m_webView.Get());
                            
                            // 注册消息接收
                            m_webView->add_WebMessageReceived(
                                Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                        wil::unique_cotaskmem_string message;
                                        args->get_WebMessageAsJson(&message);
                                        
                                        std::wstring wideMsg(message.get());
                                        std::string utf8Msg(wideMsg.begin(), wideMsg.end());
                                        
                                        m_ipcManager->OnMessageReceived(utf8Msg);
                                        
                                        return S_OK;
                                    }
                                ).Get(),
                                nullptr
                            );
                            
                            // 导航到前端页面
                            std::filesystem::path exePath = std::filesystem::current_path();
                            std::filesystem::path htmlPath = exePath / "resources" / "web" / "index.html";
                            
                            if (std::filesystem::exists(htmlPath)) {
                                std::wstring url = L"file:///" + htmlPath.wstring();
                                m_webView->Navigate(url.c_str());
                                spdlog::info("加载前端页面: {}", htmlPath.string());
                            } else {
                                // 加载开发服务器
                                m_webView->Navigate(L"http://localhost:5173");
                                spdlog::info("加载开发服务器: http://localhost:5173");
                            }
                            
                            m_webViewReady = true;
                            spdlog::info("WebView2 初始化成功");
                            
                            return S_OK;
                        }
                    ).Get()
                );
                
                return S_OK;
            }
        ).Get()
    );
}

void MainWindow::OnSize() {
    if (m_webViewController) {
        RECT bounds;
        GetClientRect(m_hwnd, &bounds);
        m_webViewController->put_Bounds(bounds);
    }
}

void MainWindow::OnDestroy() {
    m_windowManager.UnregisterWindow(m_hwnd);
    PostQuitMessage(0);
}

} // namespace CodeCanvas

