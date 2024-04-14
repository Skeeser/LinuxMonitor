#include "utils/read_file.h"

namespace monitor
{
    bool ReadFile::ReadLine(std::vector<std::string> *args)
    {
        std::string line;
        std::getline(ifs_, line);
        if (ifs_.eof() || line.empty())
        {
            return false;
        }

        std::istringstream line_ss(line);
        while (!line_ss.eof())
        {
            std::string word;
            line_ss >> word;
            args->push_back(word);
        }
        return true;
    }
    std::vector<std::string> ReadFile::GetStatsLines(const std::string &stat_file,
                                                     const int line_count)
    {
        std::vector<std::string> stats_lines;
        std::ifstream buffer(stat_file);

        // 逐行读取
        for (int line_num = 0; line_num < line_count; ++line_num)
        {
            std::string line;
            std::getline(buffer, line);
            if (line.empty())
                break;
            stats_lines.push_back(line);
        }
        return stats_lines;
    }
}