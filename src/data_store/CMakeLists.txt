
add_library(libdatastore data_store.cpp data_time.cpp system_body_factory.cpp system_body.cpp)

target_include_directories(libdatastore PRIVATE ${SRC_INCLUDE_DIR} BEFORE)
target_include_directories(libdatastore PRIVATE ${SPDLOG_INCLUDE_DIR} BEFORE)
target_include_directories(libdatastore PRIVATE ${SQLT_INCLUDE_DIR} BEFORE)
