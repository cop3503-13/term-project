#ifndef VALID_H
#define VALID_H

#include <string>
#include <regex>
#include <unordered_map>

class Valid
{
public :

    static const bool isValid(const std::string& rule, const std::string& value);

private :

    static const std::unordered_map<std::string, const std::regex> rules;
};

#endif