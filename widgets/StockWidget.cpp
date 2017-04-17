/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   StockWidget.cpp
 * Author: Phillip Jones
 *
 * Created on April 16, 2017, 6:11 PM
 */

#include "StockWidget.h"
#include <string>
#include "../include/httpreq.h"
#include "../include/jsonhttpreq.h"
#include "../include/date.h"



/***********************
 Public
 **********************/

StockWidget::StockWidget() : Widget(STOCKWIDGET_NAME)
{
    config();
}

StockWidget::StockWidget(nlohmann::json config) : Widget(STOCKWIDGET_NAME)
{
    conf = config;
}


void StockWidget::config()
{
    std::string sym = "";
    std::cout << "Please configure the Stock Widget" << std::endl;
    std::cout << "Please enter a stock symbol: " << std::endl;
    std::cout << "Symbol: ";
    std::getline(std::cin, sym);
    while(!validateSymbol(sym))
    {
        std::cout << "Please enter a stock symbol: " << std::endl;
        std::cout << "Symbol: " << std::endl;
        std::getline(std::cin, sym);
    }

    setSymbol(sym);

    conf = getConfigurationJson();

}

std::string StockWidget::getConfiguration()
{
    nlohmann::json configurationJson = getConfigurationJson();
    return configurationJson.dump(4);
}

nlohmann::json StockWidget::getConfigurationJson()
{
    nlohmann::json configurationJson = {
                             {"symbol", getSymbol()},
                             {"refreshInterval", getRefreshInterval()}
                     };

    return configurationJson;
}

nlohmann::json StockWidget::refreshData()
{
    JSONHTTPReq req = JSONHTTPReq();
    std::string url = "";
    url = getDailyUrl(conf["symbol"]);
    req.setUrl(url);
    req.send();
    nlohmann::json json = req.getJSONResponse();
    nlohmann::json transformedJson = transformResponse(json);
    return transformedJson;
}



/***********************
 Private
 **********************/

std::string StockWidget::getDailyUrl(std::string sym)
{
    return ENDPOINT + "function=" + function + "&symbol=" + sym +
           "&apikey=" + APIKEY;
}

bool StockWidget::validateSymbol(std::string sym)
{
    JSONHTTPReq req = JSONHTTPReq();
    std::string url = "";
    url = getDailyUrl(sym);
    req.setUrl(url);
    req.send();
    nlohmann::json json = req.getJSONResponse();
    bool error = true;
    try{
        json.at("Error Message");
        std::cout << "Invalid Stock Symbol" << std::endl;
    }catch(nlohmann::json::out_of_range& e){
        error = false;
    }

    return !error;
}


std::string StockWidget::getSectorUrl()
{
    return ENDPOINT + "function=SECTOR&apikey=" + APIKEY;
}

std::string StockWidget::getSymbol()
{
    return symbol;
}

void StockWidget::setSymbol(std::string sym)
{
    conf["symbol"] = symbol;
    symbol = sym;
}

nlohmann::json StockWidget::transformResponse(nlohmann::json response)
{

    date = Date::getISO8601("date");
    nlohmann::json j = response["Time Series (Daily)"];

    std::string closePrice;
    std::string openPrice;
    std::string highPrice;
    std::string lowPrice;

    for (auto it = j.begin(); it != j.end(); ++it)
    {
        nlohmann::json val = it.value();
        closePrice = val["4. close"];
        openPrice = val["1. open"];
        highPrice = val["2. high"];
        lowPrice = val["3. low"];
    }

    nlohmann::json data = {
            {"Closing Price:",  closePrice},
            {"Opening Price:",  openPrice},
            {"High Price:",     highPrice},
            {"Low Price:",      lowPrice}
    };

    return data;
}
