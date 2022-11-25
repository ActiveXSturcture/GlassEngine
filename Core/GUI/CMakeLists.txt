cmake_minimum_required(VERSION 3.20)
project(GUI)

include_directories(include)
file(GLOB_RECURSE SOUR_DIR *.cpp)

add_library(GUI SHARED ${SOUR_DIR})
if(MSVC)
    target_compile_definitions(GUI PRIVATE BUILD_DLL)
endif()

target_link_libraries(GUI imgui assimp)