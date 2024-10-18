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

add_executable(rotation_flipping rotation_flipping/rotation_flipping.cpp)

add_executable(true_type_font true_type_font/true_type_font.cpp)

add_executable(mouse_event mouse_event/mouse_event.cpp)

add_executable(key_state key_state/key_state.cpp)

add_executable(sound_effects_and_music sound_effects_and_music/sound_effects_and_music.cpp)

add_executable(timing timing/timing.cpp)

add_executable(frame_rate frame_rate/frame_rate.cpp)

add_executable(cap_frame_rate cap_frame_rate/cap_frame_rate.cpp)

add_executable(motion motion/motion.cpp)

add_executable(scrolling scrolling/scrolling.cpp)

add_executable(infinite_background infinite_background/infinite_background.cpp)

add_executable(input_and_clipboard input_and_clipboard/input_and_clipboard.cpp)

add_executable(audio_recorder audio_recorder/audio_recorder.cpp)

add_executable(window_event window_event/window_event.cpp)

add_executable(multiple_window multiple_window/multiple_window.cpp)

add_executable(text_manipulation text_manipulation/text_manipulation.cpp)

add_executable(particle_engine particle_engine/particle_engine.cpp)

add_executable(tilling tilling/tilling.cpp)

add_executable(bitmap_fonts bitmap_fonts/bitmap_fonts.cpp)

add_executable(render_to_texture render_to_texture/render_to_texture.cpp)

add_executable(frame_independent_movement frame_independent_movement/frame_independent_movement.cpp)

add_executable(timer_test test/timer_test.cpp)
