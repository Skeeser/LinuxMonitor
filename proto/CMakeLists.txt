find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(c-ares CONFIG)
# find_package(Threads)

#
# Protobuf/Grpc source files
#

if(IF_GRPC)
    set(PROTO_FILES
    monitor_info.proto
    cpu_load.proto
    cpu_softirq.proto
    cpu_stat.proto
    mem_info.proto
    net_info.proto
    rpc.proto
    )
    add_library(monitor_proto ${PROTO_FILES})
    target_link_libraries(monitor_proto
        PUBLIC
            protobuf::libprotobuf
            gRPC::grpc
            gRPC::grpc++
    )
    target_include_directories(monitor_proto PUBLIC
    ${PROTOBUF_INCLUDE_DIRS} 
    ${CMAKE_CURRENT_BINARY_DIR})

    get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
    protobuf_generate(TARGET monitor_proto LANGUAGE cpp)
    protobuf_generate(TARGET monitor_proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

else()
    set(PROTO_FILES
    cpu_load.proto
    cpu_softirq.proto
    cpu_stat.proto
    mem_info.proto
    net_info.proto
    rf_monitor_info.proto
    rpc.proto
    )

    add_library(monitor_proto ${PROTO_FILES})
    target_link_libraries(monitor_proto
        PUBLIC
            protobuf::libprotobuf
    )
    target_include_directories(monitor_proto PUBLIC
    ${PROTOBUF_INCLUDE_DIRS} 
    ${CMAKE_CURRENT_BINARY_DIR})

    protobuf_generate(TARGET monitor_proto LANGUAGE cpp)

endif()

