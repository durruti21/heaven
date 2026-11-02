

add_library(libgui gui_main.cpp gui_sim_interface.cpp gui_console.cpp gui_map.cpp gui_map_camera.cpp gui_item_display.cpp)

target_include_directories(libgui PRIVATE ${SDL3_INCLUDE_DIR} BEFORE)
target_include_directories(libgui PRIVATE ${SDL3_IMAGE_INCLUDE_DIR} BEFORE)
target_include_directories(libgui PRIVATE ${IMGUI_INCLUDE_DIR} BEFORE)
target_include_directories(libgui PRIVATE ${SRC_INCLUDE_DIR} BEFORE)
target_include_directories(libgui PRIVATE ${SPDLOG_INCLUDE_DIR} BEFORE)