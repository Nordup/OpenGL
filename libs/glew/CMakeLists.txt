# include header
target_include_directories(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# link library
if (UNIX)
    target_link_libraries(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLEW.a)
elseif (WIN32)
    target_link_libraries(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew32s.lib)
else ()
    message(STATUS "target platform cmake linking need to be added!")
endif ()