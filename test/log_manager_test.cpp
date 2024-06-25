#include "gtest/gtest.h"

#include <sstream>
#include "log_manager.h"

using namespace std;

TEST(log_manager, init_test)
{
    // 测试初始化日志
    EXPECT_NO_THROW(Log::Instance()->init(1, "./test_log", ".log", 1024));
}

TEST(log_manager, write_test)
{
    // 文档写入
}