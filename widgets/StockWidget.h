/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   StockWidget.h
 * Author: Phillip Jones
 *
 * Created on April 16, 2017, 6:02 PM
 */

#ifndef WIDGET_STOCKWIDGET_H
#define WIDGET_STOCKWIDGET_H

#define STOCKWIDGET_NAME "Stock"

#include <string>
#include "Widget.h"
#include "../include/json.hpp"

class StockWidget : public Widget
{
public:
    StockWidget();
    StockWidget(std::string config);
    StockWidget(nlohmann::json config);

    void config();
    std::string getConfiguration();
    nlohmann::json getConfigurationJson();
    nlohmann::json refreshData();

private:
    std::string date;
    std::string symbol;
    std::string function = "TIME_SERIES_DAILY";
    std::string const ENDPOINT = "http://www.alphavantage.co/query?";
    std::string const APIKEY = "8637";

    std::string getSymbol();
    void setSymbol(std::string sym);
    nlohmann::json transformResponse(nlohmann::json response);
    std::string getDailyUrl(std::string sym);
    std::string getSectorUrl();
    void configureSymbol();
    bool validateSymbol(std::string sym);


};



#endif /* WIDGET_STOCKWIDGET_H */

