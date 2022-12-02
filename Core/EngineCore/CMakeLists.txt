cmake_minimum_required(VERSION 3.20)
project(EngineCore)

include_directories(include)
file(GLOB_RECURSE SOUR_DIR *.cpp)

add_library(EngineCore SHARED ${SOUR_DIR})
if(MSVC)
    target_compile_definitions(EngineCore 
    PRIVATE BUILD_DLL
    PUBLIC GRAPHIC_DX12
    )
    target_link_libraries(EngineCore 
        d3d12
        dxgi
        dxguid
        d3dcompiler
    )
else()
target_compile_definitions(EngineCore 
    PRIVATE BUILD_DLL
    PUBLIC GRAPHIC_VULKAN
)
endif()

target_link_libraries(EngineCore glfw glm)
target_include_directories(EngineCore PUBLIC include)