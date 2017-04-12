

#ifndef TERMPROJ_WIDGET_H
#define TERMPROJ_WIDGET_H

#include <string>
#include <iostream>
#include "include/json.hpp"


class Widget {
private:
    time_t lastRefreshed = 0;
    time_t refreshInterval = 0;

public:
    //configure method prompts the user for
    //input to choose the Widget selections
    virtual void configure() = 0;

    //returns string representation of the json configuration
    virtual std::string getConfiguration() = 0;

    //returns json configuration
    virtual nlohmann::json getConfigurationJson() = 0;

    virtual std::string refreshData() = 0;

    time_t getLastRefreshed(){
        return lastRefreshed;
    }

    void setLastRefreshed(time_t lastRef)
    {
        lastRefreshed = lastRef;
    }

    time_t getRefreshInterval()
    {
        return refreshInterval;
    }

    void setRefreshInterval(time_t interval)
    {
        refreshInterval = interval;
    }

    std::string refresh()
    {
        time_t elapsed = time(0) - lastRefreshed;
        if (elapsed > getRefreshInterval())
        {
            //set last refreshed to now
            setLastRefreshed(time(0));
            return refreshData();
        }
    }
};

#endif //TERMPROJ_WIDGET_H
