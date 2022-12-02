#include "RuntimeGlobalContext.hpp"

namespace EngineCore
{
    RuntimeGlobalContext g_runtime_global_context;
    void RuntimeGlobalContext::startSystems()
    {
        m_WindowSystem = std::make_shared<WindowSystem>();
        WindowCreateInfo window_create_info;
        m_WindowSystem->initialize(window_create_info);
    }

    void RuntimeGlobalContext::shutdownSystems()
    {
        m_WindowSystem.reset();
    }
} // namespace EngineCore
