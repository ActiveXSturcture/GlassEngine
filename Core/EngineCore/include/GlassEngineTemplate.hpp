#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>
#include <queue>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <unordered_set>
#include <functional>
#include <array>
#include <stdexcept>
#ifndef ENGINECORE_DLL
#ifdef BUILD_DLL
#define ENGINECORE_DLL __declspec(dllexport)
#else
#define ENGINECORE_DLL
#endif
#endif
namespace EngineCore
{
    template <class T>
    class TSingleton
    {
    private:
        TSingleton(TSingleton &) = delete;
        TSingleton(const TSingleton &) = delete;
        TSingleton &operator=(const TSingleton &) = delete;
        static T *getInstance()
        {
            if (instance = nullptr)
            {
                instance = new T();
            }
            return instance;
        }
        static T *instance;

    protected:
        TSingleton()
        {
        }
        ~TSingleton()
        {
            delete instance;
        }
    };

    template <class T>
    T *TSingleton<T>::instance = nullptr;

    class FNocopyable
    {
    protected:
        FNocopyable() {}
        ~FNocopyable() {}

    private:
        FNocopyable(const FNocopyable &) = delete;
        FNocopyable& operator=(const FNocopyable&)=delete;
    };
} // namespace EngineCore
