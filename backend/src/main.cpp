#include "core/Application.h"
#include <Windows.h>
#include <cstdlib>

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
) {
    // 启用内存泄漏检测（Debug模式）
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    auto& app = CodeCanvas::Application::Instance();
    
    if (!app.Initialize()) {
        MessageBoxW(nullptr, L"应用程序初始化失败", L"错误", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
    
    int exitCode = app.Run();
    
    app.Shutdown();
    
    return exitCode;
}

