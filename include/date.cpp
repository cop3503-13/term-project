#include "../include/date.h"

const std::unordered_map<std::string, std::string> Date::formats = {
        {"full", "%Y-%m-%dT%H:%M:%SZ"},
        {"minute-resolution", "%Y-%m-%dT%H:%MZ"},
        {"date", "%Y-%m-%d"},
        {"time", "%H:%M:%S"},
};

const std::string Date::getISO8601(const std::string& formatId)
{
    auto t = formats.find(formatId);

    if(t != formats.end())
        return _nowStrFormat(t->second);

    throw std::out_of_range("Wrong date and time format sequence.");
}


const std::string Date::_nowStrFormat(const std::string& format)
{
    char buffer[100] = "";

    time_t now = getNow();
    struct tm * nowTm = localtime(&now);
    strftime(buffer, 100, format.c_str(), nowTm);

    return buffer;
}


const time_t Date::getNow()
{
    return time(0);
}