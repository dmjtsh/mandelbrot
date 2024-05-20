cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mandelbrot VERSION 1.0)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(mandelbrot main.cpp ${SRC_FILES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx -mavx2")

target_link_libraries(mandelbrot PRIVATE -lsfml-graphics)
target_link_libraries(mandelbrot PRIVATE -lsfml-window)
target_link_libraries(mandelbrot PRIVATE -lsfml-system)

target_include_directories(mandelbrot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
