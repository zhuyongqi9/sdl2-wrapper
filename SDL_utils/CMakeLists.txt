cmake_minimum_required(VERSION 3.2)
project(SDL_utils)

add_library(SDL_utils STATIC SDL_wrapper.cpp)
target_include_directories(SDL_utils PRIVATE ${PROJECT_SOURCE_DIR}/include)