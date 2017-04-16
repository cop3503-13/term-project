#include "valid.h"

const std::unordered_map<std::string, const std::regex> Valid::rules =
        {
                {"zipCode", std::regex("(?!0{5})(?!0{4})[0-9]{5}")},
                {"alphanumeric", std::regex("[a-zA-Z0-9]+")}
        };


const bool Valid::isValid(const std::string& rule, const std::string& value)
{
    auto t = rules.find(rule);

    if(t != rules.end())
        return std::regex_match(value, t->second);

    throw std::out_of_range("No rule defined for validation.");
}