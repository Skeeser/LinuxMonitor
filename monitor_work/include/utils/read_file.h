#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace monitor
{
    // 读取文件工具
    class ReadFile
    {
    public:
        explicit ReadFile(const std::string &name) : ifs_(name) {}

        ~ReadFile() { ifs_.close(); }

        // 读行
        bool ReadLine(std::vector<std::string> *args);
        static std::vector<std::string> GetStatsLines(const std::string &stat_file,
                                                      const int line_count);

        bool ReadLineBySplitchar(std::vector<std::string> *args, char splitchar);
        inline bool IsEnd(){
            return ifs_.eof();
        }
    private:
        std::ifstream ifs_;
    };
}