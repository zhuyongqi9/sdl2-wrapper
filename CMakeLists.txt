cmake_minimum_required(VERSION 3.2)
project(SDL2_snippets)

add_compile_definitions(MACRO_PROJECT_DIR="${PROJECT_SOURCE_DIR}")

add_subdirectory(SDL_wrapper)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

if (APPLE)
    add_compile_definitions(SCALE=1) 
else()
    add_compile_definitions(SCALE=2) 
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)    
link_libraries(SDL_wrapper)

add_executable(basic_window basic_window.cpp)

add_executable(handle_key_event handle_key_event/handle_key_event.cpp)

add_executable(stretch stretch/stretch.cpp)

add_executable(texture_png texture_png/texture_png.cpp)

add_executable(renderer_geomoetry renderer_geomoetry/renderer_geomoetry.cpp)

add_executable(renderer_viewport renderer_viewport/renderer_viewport.cpp)

add_executable(surface_color_key surface_color_key/surface_color_key.cpp)

add_executable(texture_clip texture_clip/texture_clip.cpp)

add_executable(texture_modulation texture_modulation/texture_modulation.cpp)

add_executable(texture_alpha texture_alpha/texture_alpha.cpp)

add_executable(animation animation/animation.cpp)

add_executable(texture_flipping texture_flipping/texture_flipping.cpp)

add_executable(texture_rotation texture_rotation/texture_rotation.cpp)

add_executable(ttf ttf/ttf.cpp)

add_executable(handle_mouse_event handle_mouse_event/handle_mouse_event.cpp)

add_executable(frame_rate frame_rate/frame_rate.cpp)

add_executable(frame_rate_cap frame_rate_cap/frame_rate_cap.cpp)

add_executable(background_scrolling background_scrolling/background_scrolling.cpp)

add_executable(background_infinite background_infinite/background_infinite.cpp)

add_executable(handle_input_and_clipboard handle_input_and_clipboard/handle_input_and_clipboard.cpp)

add_executable(sound_effects_and_music sound_effects_and_music/sound_effects_and_music.cpp)

add_executable(audio_recorder audio_recorder/audio_recorder.cpp)
target_include_directories(audio_recorder PRIVATE widgets/include)

add_executable(window_event window_event/window_event.cpp)

add_executable(multiple_window multiple_window/multiple_window.cpp)

add_executable(text_manipulation text_manipulation/text_manipulation.cpp)

add_executable(particle_engine particle_engine/particle_engine.cpp)

add_executable(tilling tilling/tilling.cpp)

add_executable(bitmap_fonts bitmap_fonts/bitmap_fonts.cpp)

add_executable(render_to_texture render_to_texture/render_to_texture.cpp)

add_executable(frame_independent_movement frame_independent_movement/frame_independent_movement.cpp)

add_executable(timer_test test/timer_test.cpp)

# self-made widgets

include_directories(${PROJECT_SOURCE_DIR}/widgets/include)    
add_executable(toggle_list widgets/toggle_list/toggle_list.cpp)
add_executable(selected_list widgets/selected_list/selected_list.cpp)
add_executable(recording_button widgets/recording_button/recording_button.cpp)
add_executable(playing_button widgets/playing_button/playing_button.cpp)
add_executable(text widgets/text/text.cpp)