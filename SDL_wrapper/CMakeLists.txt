cmake_minimum_required(VERSION 3.2)
project(SDL_wrapper)

set(CMAKE_CXX_STANDARD 11)

option(SDL_IMG_ENABLED OFF)
option(SDL_TTF_ENABLED OFF)
option(SDL_MIX_ENABLED OFF)
add_library(SDL_wrapper STATIC wrapper.cpp)
target_include_directories(SDL_wrapper PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(APPLE)
    target_include_directories(SDL_wrapper PUBLIC /opt/homebrew/include)
endif()

function(find_sdl_path arg1 arg2)
    if(APPLE)
        find_library(${arg1} ${arg2} /opt/homebrew/lib) 
        return()
    else()
        set(${arg1} ${arg2} PARENT_SCOPE)
        return()
    endif()
endfunction()

find_sdl_path(library SDL2)
message(${library})
target_link_libraries(SDL_wrapper PRIVATE ${library})

if (SDL_IMG_ENABLED) 
    find_sdl_path(SDL2_image_path SDL2_image)
    message(${SDL2_image_path})
    target_link_libraries(SDL_wrapper PRIVATE ${SDL2_image_path})
    target_compile_definitions(SDL_wrapper PUBLIC "SDL_IMG_ENABLED")
endif()

if (SDL_TTF_ENABLED)
    find_sdl_path(SDL2_ttf_path SDL2_ttf)
    message(${SDL2_ttf_path})
    target_link_libraries(SDL_wrapper PRIVATE ${SDL2_ttf_path})
    target_compile_definitions(SDL_wrapper PUBLIC "SDL_TTF_ENABLED")
endif()

if (SDL_MIX_ENABLED)
    find_sdl_path(SDL2_mix_path SDL2_mixer)
    message(${SDL2_mix_path})
    target_link_libraries(SDL_wrapper PRIVATE ${SDL2_mix_path})
    target_compile_definitions(SDL_wrapper PUBLIC "SDL_MIX_ENABLED")
endif()