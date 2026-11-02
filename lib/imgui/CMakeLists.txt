cmake_minimum_required(VERSION 3.10...3.21)

project(imgui VERSION 1.92.4 LANGUAGES CXX)

set(IMGUI_SRCS ./imgui.cpp ./imgui_demo.cpp ./imgui_draw.cpp ./imgui_tables.cpp ./imgui_widgets.cpp ./backends/imgui_impl_sdl3.cpp ./backends/imgui_impl_sdlrenderer3.cpp)
set(IMGUI_ALL_HEADERS ./imgui.h ./imconfig.h ./backends/imgui_impl_sdl3.h ./backends/imgui_impl_sdlrenderer3.h)

add_library(imgui STATIC ${IMGUI_SRCS} ${IMGUI_ALL_HEADERS})

add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui PRIVATE ${IMGUI_INCLUDE_DIR} BEFORE)
target_include_directories(imgui PRIVATE ${SDL3_INCLUDE_DIR} BEFORE)