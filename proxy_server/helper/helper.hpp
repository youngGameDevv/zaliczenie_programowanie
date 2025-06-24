#pragma once
#include <string>
#include <ctime>
#include "../includes/json.hpp"
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
using json = nlohmann::json;
class helper
{
    public:
    static bool isAfterCurrentTime(const std::tm& datetime_tm) 
    {

        auto datetime_time_t = std::mktime(const_cast<std::tm*>(&datetime_tm));
        auto datetime_timepoint = std::chrono::system_clock::from_time_t(datetime_time_t);
        
        auto current_time = std::chrono::system_clock::now();
        
        return datetime_timepoint > current_time;
    }

    static bool isAfterCurrentTimeS(const std::string& datetime_str) 
    {
        std::tm tm = {};
        std::istringstream ss(datetime_str);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        
        if (ss.fail()) {
            return false;
        }
        
        return isAfterCurrentTime(tm);
    }

};