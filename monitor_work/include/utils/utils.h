#pragma once
#include <boost/chrono.hpp>

namespace monitor
{
    class Utils
    {
    public:
        static double SteadyTimeSecond(
            const boost::chrono::steady_clock::time_point &t1,
            const boost::chrono::steady_clock::time_point &t2)
        {
            boost::chrono::duration<double> time_second = t1 - t2;
            return time_second.count();
        }

        static std::string trim(const std::string &str)
        {
            auto start = str.begin();
            while (start != str.end() && std::isspace(*start))
            {
                ++start;
            }

            auto end = str.end();
            do
            {
                --end;
            } while (std::distance(start, end) > 0 && std::isspace(*end));

            return std::string(start, end + 1);
        }
    };
}