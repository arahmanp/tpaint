cmake_minimum_required(VERSION 3.20)

project(TPaint)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tpaint
    src/main.cpp
    src/colors_menu.cpp
    src/canvas.cpp
    src/tpaint.cpp
    src/colors.cpp
    src/styles.cpp
)