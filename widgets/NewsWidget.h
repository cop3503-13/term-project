#ifndef NEWSWIDGET_h
#define NEWSWIDGET_h

#define NEWSWIDGET_BASEURL "https://api.nytimes.com/svc/topstories/v2/"
#define NEWSWIDGET_NAME "News"

#include "Widget.h"
#include <ctime>
#include "../include/jsonhttpreq.h"
#include "../include/json.hpp"
#include "../include/common.h"
#include "../include/valid.h"

/*
////////// Widget for the New York Times API //////////
API Key:49eb7ae0f2bb40e2a415fac4965db249
*/

typedef std::vector<std::string> List;
typedef JSONHTTPReq JReq;
typedef nlohmann::json JSON;

class NewsWidget : public Widget
{
private :

    JReq req;

    const std::string baseUrl;

    const bool doRequest(const std::string& s);

public :

    NewsWidget() : Widget(NEWSWIDGET_NAME),
                   baseUrl(NEWSWIDGET_BASEURL)
    {
        //req.setVerbose(true);
        config();
    };
    NewsWidget(JSON confData) : Widget(NEWSWIDGET_NAME),
                                baseUrl(NEWSWIDGET_BASEURL)
    {
        conf = confData;
    };

    void config() override;

    JSON refreshData() override;



};

#endif