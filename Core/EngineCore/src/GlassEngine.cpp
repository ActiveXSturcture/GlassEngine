#include "GlassEngine.hpp"

namespace EngineCore
{
    const float GlassEngine::s_fps_alpha = 1.f / 100;
    GlassEngine::GlassEngine(/* args */)
    {

    }
    GlassEngine::~GlassEngine()
    {
    }

    void GlassEngine::startEngine()
    {
        g_runtime_global_context.startSystems();
        std::cout << "Engine Start!" << std::endl;
    }
    void GlassEngine::shutdownEngine()
    {
        g_runtime_global_context.shutdownSystems();
        std::cout << "Engine Shut Down!" << std::endl;
    }
    void GlassEngine::initialize()
    {
        std::shared_ptr<WindowSystem> m_window = g_runtime_global_context.m_WindowSystem;
        assert(m_window != nullptr);

        while (!m_window->shouldClose())
        {
            const float delta_time = calculateDeltaTime();
            tickOneFrame(delta_time);
            if(delta_time < MAX_FPS)
            {
                std::chrono::milliseconds dura(static_cast<int>((MAX_FPS - delta_time)*1000));
                std::this_thread::sleep_for(dura);
            }
        }
    }

    void GlassEngine::calculateFPS(float delta_time)
    {
         m_frame_count++;

        if (m_frame_count == 1)
        {
            m_average_duration = delta_time;
        }
        else
        {
            m_average_duration = m_average_duration * (1 - s_fps_alpha) + delta_time * s_fps_alpha;
        }

        m_fps = static_cast<int>(1.f / m_average_duration);
    }
    float GlassEngine::calculateDeltaTime()
    {
        float delta_time;
        {
            using namespace std::chrono;

            steady_clock::time_point tick_time_point = steady_clock::now();
            duration<float> time_span = duration_cast<duration<float>>(tick_time_point - m_last_tick_time_point);
            delta_time = time_span.count();

            m_last_tick_time_point = tick_time_point;
        }
        return delta_time;
    }
    bool GlassEngine::tickOneFrame(float delta_time)
    {
        logicalTick(delta_time);
        calculateFPS(delta_time);
        rendererTick(delta_time);


        g_runtime_global_context.m_WindowSystem->pollEvents();
        g_runtime_global_context.m_WindowSystem->setTitle(
            std::string("GlassEngine - "+std::to_string(getFPS()) + " FPS").c_str()
        );
        const bool should_window_close = g_runtime_global_context.m_WindowSystem->shouldClose();
        return !should_window_close;
    }
    void GlassEngine::logicalTick(float delta_time)
    {
        
    }
    bool GlassEngine::rendererTick(float delta_time)
    {
        return false;
    }
} // namespace EngineCore
