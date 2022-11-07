cmake_minimum_required(VERSION 3.20)
project(RenderCore)

if(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    add_definitions(-DDEBUG)
endif()





include_directories(include)

#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOUR_DIR)
file(GLOB_RECURSE SOUR_DIR *.cpp)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_library(RenderCore SHARED ${SOUR_DIR})
if(MSVC)
    target_compile_definitions(RenderCore PRIVATE BUILD_DLL)
    target_link_libraries(RenderCore 
        d3d12
        dxgi
        dxguid
        d3dcompiler
    )
else()
    find_package(Vulkan REQUIRED)
    if(Vulkan_FOUND)
        message(STATUS "Vulkan has been Found")
    endif()
endif()

target_link_libraries(RenderCore glfw)
