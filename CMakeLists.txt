cmake_minimum_required(VERSION 3.16)
project(VirusSpreading LANGUAGES CXX)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set (SOURCES simulation.cpp main.cpp LoadMap.cpp)
set (HEADERS simulation.h LoadMap.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
)
