
set(SOURCES
    log_buffer.cpp
    log_manager.cpp
)


add_library(log_manager ${SOURCES})
target_include_directories(log_manager PUBLIC
  ${PROJECT_SOURCE_DIR}/log_manager
)


