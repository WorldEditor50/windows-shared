cmake_minimum_required(VERSION 3.5)

project(windows-shared LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
# src
file(GLOB SRC_FILES
    ${SRC_PATH}/*.h
    ${SRC_PATH}/*.hpp
    ${SRC_PATH}/*.cpp)

list(APPEND SRC_FILES main.cpp)
add_executable(windows-shared ${SRC_FILES})
