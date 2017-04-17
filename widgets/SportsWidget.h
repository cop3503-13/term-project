#ifndef SPORTSWIDGET_H
#define SPORTSWIDGET_H

/*

//////////////////////////////////// Sport Event Airings ////////////////////////////////////

API key : ssksbm4f8gr6ws6ua3vtaugf
Documentation : http://developer.tmsapi.com/io-docs
Limitations: 2 calls per second / 1,000 per day
BaseURL : http://data.tmsapi.com/v1.1/

SAMPLE GET REQUEST

http://data.tmsapi.com/v1.1/sports/58,59/events/airings?lineupsId=USA-TX42500-X&startDateTime=2017-04-14T17%3A00Z&endDateTime=2017-04-14T17%3A00Z&liveOnly=true&imageSize=Md&imageAspectTV=16x9&api_key=ssksbm4f8gr6ws6ua3vtaugf
http://data.tmsapi.com/v1.1/sports/all?includeOrg=true&api_key=1234567890
http://data.tmsapi.com/v1.1/sports/all?includeOrg=true&api_key=1234567890
*/

#include <string>
#include <unordered_map>
#include <iomanip>
#include <iostream>

#include "../include/jsonhttpreq.h"
#include "../include/json.hpp"
#include "../include/valid.h"
#include "../include/date.h"
#include "../include/common.h"

#include "Widget.h"



#define SPORTSWIDGET_BASEURL "http://data.tmsapi.com/v1.1/"
#define SPORTSWIDGET_NAME "Sports"



typedef std::vector<std::string> List;
typedef JSONHTTPReq JReq;
typedef nlohmann::json JSON;



class SportsWidget : public Widget
{
private :

    const std::string baseUrl;
    JSONHTTPReq req;

    const bool fetchSportsIds();
    const bool fetchSportsIds(const std::string& api_key);
    const bool fetchLineups();
    const bool fetchSportsAirings();

    const bool doRequest(std::string& url);
    const bool doRequest(std::string& url, const std::string& api_key);

public :

    //Constructor
    SportsWidget() : Widget(SPORTSWIDGET_NAME), baseUrl(SPORTSWIDGET_BASEURL)
    {
        config();
        setRefreshInterval(1800); //30 mins
    };

    SportsWidget(JSON confData) : Widget(SPORTSWIDGET_NAME), baseUrl(SPORTSWIDGET_BASEURL)
    {
        conf = confData;
    };

    void config() override;
    JSON refreshData() override;
};


#endif