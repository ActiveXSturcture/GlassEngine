cmake_minimum_required(VERSION 3.20)
project(GUI)

add_library( SHARED )

target_link_libraries(${CMAKE_PROJECT_NAME} imgui)