cmake_minimum_required(VERSION 3.10)
project(LaserChess3D)

set(CMAKE_CXX_STANDARD 11)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


find_package(OpenGL REQUIRED)
add_subdirectory (external)

set(
        ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS)

include_directories(
        external/glfw-3.1.2/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        ${OPENGL_INCLUDE_DIR})

add_executable(LaserChess3D
        common/Shader.h
        common/Shader.cpp
        main.cpp)

target_link_libraries(LaserChess3D ${ALL_LIBS})