#include "Widget.h"


nlohmann::json Widget::getConfJSON()
{
    return conf;
}


const std::string& Widget::getName()
{
    return name;
}



std::string Widget::refresh()
{
    time_t elapsed = time(0) - lastRefreshed;

    if (elapsed > getRefreshInterval())
    {
        //set last refreshed to now
        setLastRefreshed(time(0));
        return refreshData().dump(4);
    }

    return "";
}


time_t Widget::getLastRefreshed()
{
    return lastRefreshed;
}


void Widget::setLastRefreshed(const time_t lastRef)
{
    lastRefreshed = lastRef;
}


time_t Widget::getRefreshInterval()
{
    return refreshInterval;
}


void Widget::setRefreshInterval(const time_t interval)
{
    refreshInterval = interval;
}