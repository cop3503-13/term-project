#ifndef DATE_H
#define DATE_H

#include <string>
#include <unordered_map>

class Date
{
public :

    //methods
    static const time_t getNow();
    static const std::string getISO8601(const std::string& format = "full");

    //fields
    static const std::unordered_map<std::string, std::string> formats;

private :

    static const std::string _nowStrFormat(const std::string& format);
};

#endif