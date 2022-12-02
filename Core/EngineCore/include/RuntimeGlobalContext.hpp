#pragma once

#include "GlassEngineTemplate.hpp"
#include "WindowSystem.hpp"
namespace EngineCore
{

    class RuntimeGlobalContext : public FNocopyable
    {

    public:
        void startSystems();

        void shutdownSystems();

        std::shared_ptr<WindowSystem> m_WindowSystem;
    };
    extern RuntimeGlobalContext g_runtime_global_context;
} // namespace EngineCore