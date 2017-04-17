#include "valid.h"

const std::unordered_map<std::string, const std::regex> Valid::rules = {
        {"zipCode", std::regex("^(?!0{5})(?!0{4})[0-9]{5}$")},
        {"alphanumeric", std::regex("^([a-zA-Z]|\\d)+$")},
        {"int", std::regex("^\\d+$")},
        {"csvInts", std::regex("^\\d+([,]\\d+)*$")},
        {"csvIntsSpc", std::regex("^\\s*\\d+\\s*([,]\\s*\\d+\\s*)*$")},
        {"csvIntsSpc1-5", std::regex("^\\s*[1-5]\\s*([,]\\s*[1-5]+\\s*)*$")},
        {"csvAlpha", std::regex("^[a-zA-Z]+([,][a-zA-Z]+)*$")},
        {"csvAlphaSpc", std::regex("^\\s*[a-zA-Z]+\\s*([,]\\s*([a-zA-Z]|\\s)+\\s*)*$")},
};


const bool Valid::isValid(const std::string& rule, const std::string& value)
{
    auto t = rules.find(rule);

    if(t != rules.end())
        return std::regex_match(value, t->second);

    throw std::out_of_range("No rule defined for validation.");
}