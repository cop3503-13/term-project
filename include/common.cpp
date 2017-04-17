#include "common.h"
#include <iostream>
#include <algorithm>

List& Common::pushList(List& target, const List& source)
{
    target.insert(target.end(), source.begin(), source.end());
    return target;
}


List Common::split(const std::string& s, const std::string& delimiter)
{
    List output;
    size_t at = s.find(delimiter);

    if(at == std::string::npos)
        output.push_back(s);
    else
    {
        output.push_back(s.substr(0, at));
        pushList(output, split(s.substr(at + delimiter.length()), delimiter));
    }

    return output;
}


std::string Common::join(const List& list, const std::string& delimiter)
{
    std::stringstream ss;

    for(List::const_iterator it = list.begin(); it != list.end(); it++)
    {
        if(it != list.begin())
            ss << delimiter;

        ss << *it;
    }

    return ss.str();
}

bool Common::contains(const List& list, const std::string& search)
{
    if (std::find(list.begin(), list.end(), search) != list.end())
        return true;
    return false;
}