cmake_minimum_required(VERSION 3.10.2)
project(monitor_work LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)


option(IF_GRPC "IF USE gRpc?" OFF)

add_subdirectory(log_manager)
add_subdirectory(proto)
add_subdirectory(muduo)
add_subdirectory(rpc_manager)
add_subdirectory(monitor_work)
add_subdirectory(rpc_framework)
# add_subdirectory(display_monitor)
