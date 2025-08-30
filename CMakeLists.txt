cmake_minimum_required(VERSION 3.28)
project(lagger)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lagger)

target_sources(lagger PRIVATE
    ${CMAKE_SOURCE_DIR}/src/gui.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/utils.cpp
)

target_include_directories(lagger PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/lagger
)


