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
    static List split(const std::string& str, const std::string& delimiter = ",");
};

#endif //COMMON_H