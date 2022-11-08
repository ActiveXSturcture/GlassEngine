cmake_minimum_required(VERSION 3.20)
project(third_party LANGUAGES C CXX)


#imgui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ImGUI")
set(IMGUI_FILES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imconfig.h"
    "${IMGUI_DIR}/imgui.h"
    "${IMGUI_DIR}/imgui_internal.h"
    "${IMGUI_DIR}/imstb_rectpack.h"
    "${IMGUI_DIR}/imstb_textedit.h"
    "${IMGUI_DIR}/imstb_truetype.h")

add_library(imgui STATIC ${IMGUI_FILES})

target_include_directories(imgui PUBLIC ${IMGUI_DIR})

#GLFW
add_subdirectory(GLFW)

#AssImp
set(ASSIMP_INSTALL OFF)
#add_subdirectory(Assimp)