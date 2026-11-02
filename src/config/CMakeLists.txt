add_library(libconfig config_data.cpp config_setup.cpp config_application.cpp)


target_include_directories(libconfig PRIVATE ${JSON_INCLUDE_DIR} BEFORE)
target_include_directories(libconfig PRIVATE ${SRC_INCLUDE_DIR} BEFORE)
target_include_directories(libconfig PRIVATE ${SPDLOG_INCLUDE_DIR} BEFORE)