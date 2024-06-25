

set(SOURCES
    alltest_main.cpp
    log_manager_test.cpp
    rpc_client_test.cpp
)

add_executable(test ${SOURCES})

target_link_libraries(test
    PUBLIC
    log_manager
    client
)


find_package(GTest CONFIG REQUIRED)
target_link_libraries(test PUBLIC GTest::gtest GTest::gtest_main ${LIB_FUC} )