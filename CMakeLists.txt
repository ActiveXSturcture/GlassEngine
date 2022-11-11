cmake_minimum_required(VERSION 3.20)
Project(GlassEngine VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Core/ThirdPartys)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Core/RenderCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/RenderCore/include)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)


target_link_libraries(${CMAKE_PROJECT_NAME} RenderCore)