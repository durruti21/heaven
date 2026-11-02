cmake_minimum_required(VERSION 3.28)

project(
    Heaven 
    VERSION 0.1 
    LANGUAGES C CXX) 

set(JSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/nlohmann_json/include")    
set(SDL3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3/include")
set(SDL3_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image/include")
set(SQLT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/sqlite/bld")
set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/imgui/")
set(SPDLOG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/spdlog/include")
set(SRC_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src")
set(JSON_BuildTets OFF CACHE INTERNAL "")


set(JSON_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib/nlohmann_json")
set(SDL3_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3/lib64")
set(SDL3_IMAGE_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image/lib64")
set(SQLT_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib/sqlite/bld")

add_executable(heaven src/heaven.cpp)
add_subdirectory(src/support)
add_subdirectory(src/config)
add_subdirectory(src/gui)
add_subdirectory(src/data_store)
add_subdirectory(src/engine)
add_subdirectory(lib/nlohmann_json)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/imgui)

target_include_directories(heaven PRIVATE ${SDL3_INCLUDE_DIR} BEFORE)
target_include_directories(heaven PRIVATE ${SDL3_IMAGE_INCLUDE_DIR} BEFORE)
target_include_directories(heaven PRIVATE ${SRC_INCLUDE_DIR} BEFORE)

target_link_libraries(heaven PRIVATE ${SDL3_LIBRARY_DIR}/libSDL3.so)
target_link_libraries(heaven PRIVATE ${SDL3_IMAGE_LIBRARY_DIR}/libSDL3_image.so)
target_link_libraries(heaven PRIVATE ${SQLT_LIBRARY_DIR}/libsqlite3.so)
target_link_libraries(heaven PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(heaven PRIVATE spdlog::spdlog)
target_link_libraries(heaven PRIVATE libconfig libengine libgui libdatastore libsupport)
target_link_libraries(heaven PRIVATE imgui::imgui)