#pragma once

#include <memory>
#include <string>
#include "WindowManager.h"

namespace CodeCanvas {

class Application {
public:
    static Application& Instance();
    
    bool Initialize();
    int Run();
    void Shutdown();
    
    WindowManager& GetWindowManager() { return *m_windowManager; }
    
private:
    Application() = default;
    ~Application() = default;
    
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    
    void InitializeLogging();
    
    std::unique_ptr<WindowManager> m_windowManager;
    bool m_isRunning = false;
};

} // namespace CodeCanvas

