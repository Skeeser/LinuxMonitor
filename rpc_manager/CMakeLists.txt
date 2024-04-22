#
# Dependencies
#
# find_package(Threads)

#
# Sources
#

# 设置rpc的种类

#  SET(RPC_TYPE "gRpc")
# SET(RPC_TYPE "rpcFramework")

add_subdirectory(server)
add_subdirectory(client)