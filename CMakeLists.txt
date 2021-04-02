# project settings
project(OpenGL)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

# executable name
set(EXECUTABLE opengl)
set(SOURCES src/main.cpp src/Renderer.cpp src/VertexBuffer.cpp
            src/IndexBuffer.cpp src/VertexArray.cpp src/Shader.cpp
            src/InitializeLibs.cpp src/Texture.cpp)

# executables
add_executable(${EXECUTABLE} ${SOURCES})
add_compile_definitions(PROJECT_DIR=${CMAKE_SOURCE_DIR})

# headers
# include_directories(include/)

# add libraries
add_subdirectory(libs/glew)
add_subdirectory(libs/GLFW)
# vendor
add_subdirectory(src/vendor)

# preprocessor definition for GLEW
add_compile_definitions(GLEW_STATIC)