# include header
target_include_directories(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# link library
target_link_libraries(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/glew32s.lib opengl32)