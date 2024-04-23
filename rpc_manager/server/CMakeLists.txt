
if(IF_GRPC)
    set(SOURCES
        grpc_main.cpp
        grpc_manager.cpp
    )
else()
    set(SOURCES
        rpc_framework_manager.cpp
        rpc_framework_main.cpp
    )
endif()

add_executable(server ${SOURCES}) 
target_link_libraries(server 
    PUBLIC 
    monitor_proto 
    log_manager 
    rpc_framework
)

