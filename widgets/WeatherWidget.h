
#ifndef WIDGET_WEATHERWIDGET_H
#define WIDGET_WEATHERWIDGET_H

#include <string>
#include "Widget.h"
#include "../include/json.hpp"

class WeatherWidget : public Widget {
public:
    WeatherWidget();
    WeatherWidget(std::string config);
    WeatherWidget(nlohmann::json config);

    void configure();
    std::string getConfiguration();
    nlohmann::json getConfigurationJson();
    nlohmann::json refreshData();

private:
    std::string zipcode;
    std::string const ENDPOINT = "http://api.openweathermap.org/data/2.5/weather";
    std::string const APIKEY = "edc7afc82b7d745a80be3761e51b3055";


    std::string getZipCode();
    void setZipCode(std::string zip);
    nlohmann::json transformResponse(nlohmann::json response);
    std::string getDailyUrl(std::string zip);
    std::string get5dayUrl(std::string zip);
    void configureZipCode();
    bool validZipCode(std::string zip);

};


#endif //WIDGET_WEATHERWIDGET_H