#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <sstream>
#include <vector>

typedef std::vector<std::string> List;

class Common
{
public :

    //METHODS
    static List& pushList(List& target, const List& source);
    static std::string join(const List& list, const std::string& delimiter = ",");
    static List split(const std::string& str, const std::string& delimiter = ",", const bool trim = false);
    static List split(const std::string& s, const bool trimWhitespace);
    static const bool contains(const List& list, const std::string& search, const bool trimWhitespace = false);
    static const bool contains(const List& list, const List& listMatch, const bool trimWhitespace = false);
    static std::string trim(const std::string& s);
    static std::string trim(const std::string& s, const std::string& w);


    //FIELDS
    static const std::string whitespace;
};

#endif //COMMON_H