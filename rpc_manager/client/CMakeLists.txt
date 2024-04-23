


if(IF_GRPC)
    set(SOURCES
    # main.cpp
    grpc_client.cpp
)
else()
    set(SOURCES
        rpc_framework_client.cpp
    )
endif()

# add_executable(client ${SOURCES})
# client 作为库使用

add_library(client  ${SOURCES})
target_link_libraries(client
    PUBLIC
    monitor_proto
    log_manager
    rpc_framework
    network
)

