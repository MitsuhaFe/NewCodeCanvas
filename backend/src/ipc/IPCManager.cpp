#include "ipc/IPCManager.h"
#include <spdlog/spdlog.h>
#include <WebView2.h>

namespace CodeCanvas {

IPCManager::IPCManager(ICoreWebView2* webView)
    : m_webView(webView) {
    
    // 注册系统方法
    RegisterMethod("system.getInfo", [](const nlohmann::json& params) {
        nlohmann::json result;
        result["appVersion"] = "1.0.0-dev";
        result["platform"] = "Windows";
        return result;
    });
    
    spdlog::info("IPC Manager 初始化完成");
}

IPCManager::~IPCManager() {
}

void IPCManager::RegisterMethod(const std::string& method, MessageHandler handler) {
    m_handlers[method] = handler;
    spdlog::debug("注册 IPC 方法: {}", method);
}

void IPCManager::UnregisterMethod(const std::string& method) {
    m_handlers.erase(method);
    spdlog::debug("注销 IPC 方法: {}", method);
}

void IPCManager::PostMessage(const std::string& method, const nlohmann::json& params) {
    nlohmann::json message;
    message["jsonrpc"] = "2.0";
    message["method"] = method;
    message["params"] = params;
    
    std::string jsonStr = message.dump();
    std::wstring wideStr(jsonStr.begin(), jsonStr.end());
    
    m_webView->PostWebMessageAsJson(wideStr.c_str());
}

void IPCManager::PostEvent(const std::string& event, const nlohmann::json& data) {
    PostMessage(event, data);
}

void IPCManager::OnMessageReceived(const std::string& message) {
    try {
        auto json = nlohmann::json::parse(message);
        
        if (json.contains("id") && json.contains("method")) {
            HandleRequest(json);
        }
        
    } catch (const std::exception& e) {
        spdlog::error("处理消息失败: {}", e.what());
    }
}

void IPCManager::HandleRequest(const nlohmann::json& request) {
    int id = request["id"];
    std::string method = request["method"];
    nlohmann::json params = request.value("params", nlohmann::json::object());
    
    spdlog::debug("处理 IPC 请求: {} (id: {})", method, id);
    
    auto it = m_handlers.find(method);
    if (it != m_handlers.end()) {
        try {
            nlohmann::json result = it->second(params);
            nlohmann::json response = CreateResponse(id, result);
            
            std::string jsonStr = response.dump();
            std::wstring wideStr(jsonStr.begin(), jsonStr.end());
            m_webView->PostWebMessageAsJson(wideStr.c_str());
            
        } catch (const std::exception& e) {
            spdlog::error("执行方法失败: {}", e.what());
            nlohmann::json response = CreateError(id, -32603, e.what());
            
            std::string jsonStr = response.dump();
            std::wstring wideStr(jsonStr.begin(), jsonStr.end());
            m_webView->PostWebMessageAsJson(wideStr.c_str());
        }
    } else {
        spdlog::warn("未找到方法: {}", method);
        nlohmann::json response = CreateError(id, -32601, "Method not found");
        
        std::string jsonStr = response.dump();
        std::wstring wideStr(jsonStr.begin(), jsonStr.end());
        m_webView->PostWebMessageAsJson(wideStr.c_str());
    }
}

nlohmann::json IPCManager::CreateResponse(int id, const nlohmann::json& result) {
    nlohmann::json response;
    response["jsonrpc"] = "2.0";
    response["result"] = result;
    response["id"] = id;
    return response;
}

nlohmann::json IPCManager::CreateError(int id, int code, const std::string& message) {
    nlohmann::json response;
    response["jsonrpc"] = "2.0";
    response["error"]["code"] = code;
    response["error"]["message"] = message;
    response["id"] = id;
    return response;
}

} // namespace CodeCanvas

