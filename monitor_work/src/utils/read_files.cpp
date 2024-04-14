#include "read_file.h"

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
    }
    std::vector<std::string> ReadFile::GetStatsLines(const std::string &stat_file,
                                                     const int line_count)
    {
    }
}