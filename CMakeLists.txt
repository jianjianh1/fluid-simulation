cmake_minimum_required(VERSION 3.20)
project(
    fluid-simulation
    VERSION 1.0.0
    LANGUAGES C CXX
    )
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3)
add_subdirectory("external")

add_subdirectory("src")

add_subdirectory("shaders")
add_subdirectory("assets")

add_executable(main app/main.cpp)
target_link_libraries(
    main PRIVATE
    renderer
    )
