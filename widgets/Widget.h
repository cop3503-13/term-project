

#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <iostream>
#include "../include/json.hpp"


class Widget {
private:
    time_t lastRefreshed = 0;
    time_t refreshInterval = 600;

public:
    virtual std::string getName();

    //configure method prompts the user for
    //input to choose the Widget selections
    virtual void configure() = 0;

    //returns json configuration
    // This should be a json formatted like this:
    // {
    //    "name": "widget name",
    //    "configuration": {configuration options here}
    //  }
    virtual nlohmann::json getConfigurationJson() = 0;

    //returns json configuration
    // this will switch to"
    // virtual nlohmann::json  refreshData() = 0
    // this should return json in the format:
    //     {  
    //        "name": "name of widget"
    //        "data": {json data we want from api}
    //      }
    virtual nlohmann::json refreshData() = 0;

    time_t getLastRefreshed(){
        return lastRefreshed;
    }

    void setLastRefreshed(time_t lastRef)
    {
        lastRefreshed = lastRef;
    }


    /*******
     * Gets the interval that the data should be updated
     * override this to provide a refresh interval specific
     * to your object
     *
     * @return time_t refresh interval
     */

    virtual time_t getRefreshInterval()
    {
        return refreshInterval;
    }



    void setRefreshInterval(time_t interval)
    {
        refreshInterval = interval;
    }

    std::string refresh()
    {
        time_t elapsed = time(0) - getLastRefreshed();
        if (elapsed > getRefreshInterval())
        {
            //set last refreshed to now
            setLastRefreshed(time(0));
            return refreshData();
        }
        return "";
    }
};

#endif //WIDGET_H
