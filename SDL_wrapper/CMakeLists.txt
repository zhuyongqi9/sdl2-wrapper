cmake_minimum_required(VERSION 3.2)
project(SDL_wrapper)

set(CMAKE_CXX_STANDARD 11)
add_library(SDL_wrapper STATIC wrapper.cpp)
target_include_directories(SDL_wrapper PRIVATE ${PROJECT_SOURCE_DIR}/include)