# include header
target_include_directories(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# link library
if (UNIX)
    target_link_libraries(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a OpenGL dl X11 GL pthread)
elseif (WIN32)
    target_link_libraries(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib opengl)
else ()
    message(STATUS "target platform cmake linking need to be added!")
endif ()