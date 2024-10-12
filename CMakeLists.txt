cmake_minimum_required(VERSION 3.2)
project(SDL2_snippets)

if(APPLE)
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif()

add_compile_definitions(MACRO_PROJECT_DIR="${PROJECT_SOURCE_DIR}")

add_subdirectory(SDL_utils)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/SDL_utils/include)
include_directories(${PROJECT_SOURCE_DIR}/include)    
link_libraries(SDL_utils)

add_executable(basic_window basic_window.cpp)
target_link_libraries(basic_window PRIVATE SDL2)

add_executable(key_handler key_handler/key_handler.cpp)
target_link_libraries(key_handler PRIVATE SDL2)

add_executable(stretch stretch/stretch.cpp)
target_link_libraries(stretch PRIVATE SDL2)

add_executable(png png/png.cpp)
target_link_libraries(png PRIVATE SDL2 SDL2_image)

add_executable(texture_render texture_render/texture_render.cpp)
target_link_libraries(texture_render PRIVATE SDL2 SDL2_image)

add_executable(geomoetry_render geomoetry_render/geomoetry_render.cpp)
target_link_libraries(geomoetry_render PRIVATE SDL2 SDL2_image)

add_executable(view_point view_point/view_point.cpp)
target_link_libraries(view_point PRIVATE SDL2 SDL2_image)

add_executable(color_keying color_keying/color_keying.cpp)
target_link_libraries(color_keying PRIVATE SDL2 SDL2_image)

add_executable(clip_rendering clip_rendering/clip_rendering.cpp)
target_link_libraries(clip_rendering PRIVATE SDL2 SDL2_image)

add_executable(color_modulation color_modulation/color_modulation.cpp)
target_link_libraries(color_modulation PRIVATE SDL2 SDL2_image)

add_executable(alpha_blending alpha_blending/alpha_blending.cpp)
target_link_libraries(alpha_blending PRIVATE SDL2 SDL2_image)

add_executable(animation animation/animation.cpp)
target_link_libraries(animation PRIVATE SDL2 SDL2_image)

add_executable(rotation_flipping rotation_flipping/rotation_flipping.cpp)
target_link_libraries(rotation_flipping PRIVATE SDL2 SDL2_image)

add_executable(true_type_font true_type_font/true_type_font.cpp)
target_link_libraries(true_type_font PRIVATE SDL2 SDL2_image SDL2_ttf)

add_executable(mouse_event mouse_event/mouse_event.cpp)
target_link_libraries(mouse_event PRIVATE SDL2 SDL2_image SDL2_ttf)

add_executable(key_state key_state/key_state.cpp)
target_link_libraries(key_state PRIVATE SDL2 SDL2_image SDL2_ttf)

add_executable(sound_effects_and_music sound_effects_and_music/sound_effects_and_music.cpp)
target_link_libraries(sound_effects_and_music PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(timing timing/timing.cpp)
target_link_libraries(timing PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(frame_rate frame_rate/frame_rate.cpp)
target_link_libraries(frame_rate PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(cap_frame_rate cap_frame_rate/cap_frame_rate.cpp)
target_link_libraries(cap_frame_rate PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(motion motion/motion.cpp)
target_link_libraries(motion PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(scrolling scrolling/scrolling.cpp)
target_link_libraries(scrolling PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(infinite_background infinite_background/infinite_background.cpp)
target_link_libraries(infinite_background PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(input_and_clipboard input_and_clipboard/input_and_clipboard.cpp)
target_link_libraries(input_and_clipboard PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(audio_recorder audio_recorder/audio_recorder.cpp)
target_link_libraries(audio_recorder PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(window_event window_event/window_event.cpp)
target_link_libraries(window_event PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(multiple_window multiple_window/multiple_window.cpp)
target_link_libraries(multiple_window PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(text_manipulation text_manipulation/text_manipulation.cpp)
target_link_libraries(text_manipulation PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(particle_engine particle_engine/particle_engine.cpp)
target_link_libraries(particle_engine PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(tilling tilling/tilling.cpp)
target_link_libraries(tilling PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(bitmap_fonts bitmap_fonts/bitmap_fonts.cpp)
target_link_libraries(bitmap_fonts PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(render_to_texture render_to_texture/render_to_texture.cpp)
target_link_libraries(render_to_texture PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_executable(timer_test test/timer_test.cpp)
