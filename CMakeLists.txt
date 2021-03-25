# project settings
project(OpenGL)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

# executable name
set(EXECUTABLE opengl)

# executables
add_executable(${EXECUTABLE} src/Application.cpp)
add_compile_definitions(PROJECT_DIR=${CMAKE_SOURCE_DIR})

# add libraries
add_subdirectory(libs/glew)
add_subdirectory(libs/GLFW)
# preprocessor definition for GLEW
add_compile_definitions(GLEW_STATIC)