
add_library(libsupport logger.cpp types/unit_types.cpp)

target_include_directories(libsupport PRIVATE ${SPDLOG_INCLUDE_DIR} BEFORE)
target_include_directories(libsupport PRIVATE ${SRC_INCLUDE_DIR} BEFORE)
