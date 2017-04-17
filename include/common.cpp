#include "common.h"
#include <iostream>
#include <algorithm>


const std::string Common::whitespace = " \t\f\v\n\r";


List& Common::pushList(List& target, const List& source)
{
    target.insert(target.end(), source.begin(), source.end());
    return target;
}




List Common::split(const std::string& s, const bool trimWhitespace)
{
    return split(s, ",", trimWhitespace);
}

List Common::split(const std::string& s, const std::string& delimiter, const bool trimWhitespace)
{
    List output;
    size_t at = s.find(delimiter);

    if(at == std::string::npos)
        output.push_back(trimWhitespace ? trim(s) : s);
    else
    {
        output.push_back(trimWhitespace ? trim(s.substr(0, at)) : s.substr(0, at));
        pushList(output, split(s.substr(at + delimiter.length()), delimiter, trimWhitespace));
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



const bool Common::contains(const List& list, const std::string& search, const bool trimWhitespace)
{
    return std::find(list.begin(), list.end(), trimWhitespace ? trim(search) : search) != list.end();
}


const bool Common::contains(const List& list, const List& listMatch, const bool trimWhitespace)
{
    for(const std::string& item : listMatch)
    {
        if(!contains(list, item, trimWhitespace))
            return false;
    }

    return true;
}




std::string Common::trim(const std::string& s)
{
    return trim(s, whitespace);
}


std::string Common::trim(const std::string& s, const std::string& w)
{
    size_t begin = s.find_first_not_of(w);

    // only whitespaces
    if(begin == std::string::npos)
        return "";

    size_t end = s.find_last_not_of(w);

    std::cout << "TEST ->" << s.substr(begin, end - begin + 1) << "|" << std::endl;

    return s.substr(begin, end - begin + 1);
}