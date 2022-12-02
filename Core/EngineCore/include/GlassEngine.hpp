#pragma once
#include "GlassEngineTemplate.hpp"
#include "RuntimeGlobalContext.hpp"

#define MAX_FPS (1.0f/60.0f)
namespace EngineCore
{
    class ENGINECORE_DLL GlassEngine : public TSingleton<GlassEngine>
    {
        friend class TSingleton<GlassEngine>;
        static const float s_fps_alpha;
    protected:
        std::chrono::steady_clock::time_point m_last_tick_time_point {std::chrono::steady_clock::now()};
        float m_average_duration {0.f};
        int   m_frame_count {0};
        int   m_fps {0};
        bool m_is_quit {false};
    public:
        GlassEngine(/* args */);
        ~GlassEngine();

        void startEngine();
        void shutdownEngine();

        void initialize();
        void clear();

        bool isQuit() const { return m_is_quit; }
        void run();
        bool tickOneFrame(float delta_time);
        int getFPS() const { return m_fps; }

    protected:
        void logicalTick(float delta_time);
        bool rendererTick(float delta_time);

        void calculateFPS(float delta_time);

        /**
         *  Each frame can only be called once
         */
        float calculateDeltaTime();
    };
    
    
} // namespace Glass
