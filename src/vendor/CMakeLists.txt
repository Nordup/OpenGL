# include header
target_include_directories(${EXECUTABLE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# source
target_sources(${EXECUTABLE} PUBLIC stb_image/stb_image.cpp)
