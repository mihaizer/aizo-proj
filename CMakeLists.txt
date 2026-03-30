cmake_minimum_required(VERSION 3.31)

project(aizo_proj LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(aizo_proj
  src/main.cpp
)

target_include_directories(aizo_proj PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
