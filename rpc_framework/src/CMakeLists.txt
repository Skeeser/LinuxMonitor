set(SOURCES
    rpc_channel.cpp
    rpc_server.cpp
    rpc_codec.cpp
)

add_library(rpc_framework ${SOURCES})

target_include_directories(rpc_framework PUBLIC
  ${PROJECT_SOURCE_DIR}/rpc_framework/include
)

target_link_libraries(rpc_framework
    PUBLIC
    network
    monitor_proto
    protobuf
    z
    log_manager
)


# install(TARGETS  rpc_framework DESTINATION  ${PROJECT_BINARY_DIR}/lib)