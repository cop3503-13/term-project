#ifndef MOVIEWIDGET_H
#define MOVIEWIDGET_H

#define MOVIEWIDGET_BASEURL "http://data.tmsapi.com/v1.1/movies/showings"
#define MOVIEWIDGET_NAME "Movie"

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include "Widget.h"
#include "../include/json.hpp"
#include "../include/jsonhttpreq.h"
#include "../include/date.h"
#include "../include/valid.h"


typedef nlohmann::json JSON;

class MovieWidget : public Widget
{

private:

    const std::string baseUrl;
    JSONHTTPReq req;

public:

    //Constructor
    MovieWidget() : Widget(MOVIEWIDGET_NAME),
                    baseUrl(MOVIEWIDGET_BASEURL)
    {
        config();
        setRefreshInterval(1800); //30 mins
    };

    MovieWidget(JSON confData) :
            Widget(MOVIEWIDGET_NAME),
            baseUrl(MOVIEWIDGET_BASEURL)
    {
        conf = confData;
    };

    void doRequest(const std::string& url);

    //Mandatory methods from base widget class
    void config() override;
    JSON refreshData() override;

    //Other, non-trivial methods
    const bool isAPIkeyValid(const std::string& api_key);
};

#endif

