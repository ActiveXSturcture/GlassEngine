cmake_minimum_required(VERSION 3.20)
project(RenderCore)
include_directories(include)
include_directories()
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOUR_DIR)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_library(RenderCore SHARED ${SOUR_DIR})
if(MSVC)
    target_compile_definitions(RenderCore PRIVATE BUILD_DLL)
    target_link_libraries(RenderCore 
        d3d12.lib
        dxgi.lib
        dxguid.lib
        d3dcompiler.lib
    )
endif()

target_link_libraries(RenderCore glfw)
