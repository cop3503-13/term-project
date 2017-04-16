

#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <iostream>
#include "../include/json.hpp"


class Widget {
private:
    time_t lastRefreshed = 0;
    time_t refreshInterval = 600;

protected:
    std::string const name;
    nlohmann::json conf;

public:

    Widget(std::string const &nameIt) : name(nameIt){};
    virtual ~Widget(){};

    //returns json configuration
    // This should be a json formatted like this:
    // {
    //    "name": "widget name",
    //    "configuration": {configuration options here}
    //  }
    nlohmann::json getConfJSON();

    const std::string& getName();

    time_t getLastRefreshed();
    void setLastRefreshed(time_t lastRef);


    /*******
     * Gets the interval that the data should be updated
     * override this to provide a refresh interval specific
     * to your object
     *
     * @return time_t refresh interval
     */

    time_t getRefreshInterval();
    void setRefreshInterval(time_t interval);

    std::string refresh();

    /******************
     * VIRTUAL METHODS TO BE OVERRIDEN ON INHERITANCE
     */


    //config method prompts the user for
    //input to choose the Widget selections
    virtual void config() = 0;

    //returns json configuration
    // this should return json in the format:
    //     {
    //        "name": "name of widget"
    //        "data": {json data we want from api}
    //      }
    virtual nlohmann::json refreshData() = 0;

};

#endif //WIDGET_H
