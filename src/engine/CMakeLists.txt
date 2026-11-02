add_library(libengine sim_engine.cpp)

target_include_directories(libengine PRIVATE ${SRC_INCLUDE_DIR} BEFORE)
target_include_directories(libengine PRIVATE ${SPDLOG_INCLUDE_DIR} BEFORE)
