#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>
#include <queue>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#ifndef GUI_DLL
    #ifdef BUILD_DLL
        #define GUI_DLL __declspec(dllexport)
    #else
        #define GUI_DLL
    #endif
#endif
