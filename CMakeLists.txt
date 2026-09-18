cmake_minimum_required(VERSION 3.20)

project(AStarPathfinding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(AStarPathfinding
    src/main.cpp
)

target_include_directories(AStarPathfinding PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(AStarPathfinding PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)