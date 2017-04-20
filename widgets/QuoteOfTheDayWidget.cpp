#include <ctime>
#include <iostream>
#include <string>
#include "QuoteOfTheDayWidget.h"
#include "Widget.h"
#include "../include/httpreq.h"
#include "../include/jsonhttpreq.h"

QuoteOfTheDayWidget::QuoteOfTheDayWidget() : Widget(QODWIDGET_NAME) {

    quote = "";
    author = "";
    quoteOptionType = "";
    baseUrl = "http://quotes.rest/qod.json";

    time_t rfreshInterval = 24 * 60 * 60;        // overriding default refresh interval of 10 minutes to 24 hours
    setRefreshInterval(rfreshInterval);
//    time_t lstRefreshed = 0;
//    this->setLastRefreshed(lstRefreshed);
//    conf = this->getConfigurationJson();
    config();
}

QuoteOfTheDayWidget::QuoteOfTheDayWidget(std::string qteTypeOption) : Widget(QODWIDGET_NAME) {
    quote = "";
    author = "";

    time_t rfreshInterval = 24 * 60 * 60;        // overriding default refresh interval of 10 minutes to 24 hours
    this->setRefreshInterval(rfreshInterval);
    time_t lstRefreshed = 0;
    this->setLastRefreshed(lstRefreshed);

    quoteOptionType = qteTypeOption;
    if (qteTypeOption != "")
        baseUrl = "http://quotes.rest/qod.json?category=" + qteTypeOption;
    else
        baseUrl = "http://quotes.rest/qod.json";
    conf = this->getConfigurationJson();
}

QuoteOfTheDayWidget::QuoteOfTheDayWidget(nlohmann::json conf) : Widget(QODWIDGET_NAME) {
    quote = "";
    author = "";

    time_t rfreshInterval = 24 * 60 * 60;        // overriding default refresh interval of 10 minutes to 24 hours
    this->setRefreshInterval(rfreshInterval);
    time_t lstRefreshed = 0;
    this->setLastRefreshed(lstRefreshed);
    std::string qteTypeOption = conf["quoteOptionType"];

    quoteOptionType = qteTypeOption;
    if (qteTypeOption != "")
        baseUrl = "http://quotes.rest/qod.json?category=" + qteTypeOption;
    else
        baseUrl = "http://quotes.rest/qod.json";
    conf = this->getConfigurationJson();
}

QuoteOfTheDayWidget::~QuoteOfTheDayWidget() {}        //destructor

nlohmann::json QuoteOfTheDayWidget::getConfigurationJson() {
    nlohmann::json conf_json = {{"quoteOptionType", this->getQuoteTypeOption()}};
    return conf_json;
}


void QuoteOfTheDayWidget::config() {  // this is the part that prompts the user asking for the quote type
    std::string userInput;
    std::string quoteOptionType;
    bool userDone = false;

    while (!userDone) {
        std::cout << "The following options are available for the Quote of the Day \n" << std::endl;
        std::cout << "\t 1: Inspire:    Inspiring Quote of the day " << std::endl;
        std::cout << "\t 2: Management: Management Quote of the day" << std::endl;
        std::cout << "\t 3: Sports:     Sports Quote of the day" << std::endl;
        std::cout << "\t 4: Life:       Quote of the day about life" << std::endl;
        std::cout << "\t 5: Funny:      Funny Quote of the day" << std::endl;
        std::cout << "\t 6: Love:       Quote of the day about Love" << std::endl;
        std::cout << "\t 7: Art:        Art quote of the day " << std::endl;
        std::cout << "\t 8: Students:   Quote of the day for students" << std::endl;
        std::cout << "\t 9: Random:     Quote of the day selected randomly from the above categories (default)"
                  << std::endl;
        std::cout << "\n Please enter a number 1 - 9 to select your quote type (default 9)" << std::endl;

        std::getline(std::cin, userInput);
        char ui;
        ui = userInput[0];

        switch (ui) {
            case '1':
                quoteOptionType = "inspire";
                break;

            case '2':
                quoteOptionType = "management";
                break;

            case '3':
                quoteOptionType = "sports";
                break;

            case '4':
                quoteOptionType = "life";
                break;

            case '5':
                quoteOptionType = "funny";
                break;

            case '6':
                quoteOptionType = "love";
                break;

            case '7':
                quoteOptionType = "art";
                break;

            case '8':
                quoteOptionType = "students";
                break;

            case '9':
                quoteOptionType = "";
                break;

            default:
                quoteOptionType = "";
                break;
        }    //end of switch statement

        std::string quoteOptionTypePrompt;

        if (quoteOptionType != "") quoteOptionTypePrompt = quoteOptionType;
        else quoteOptionTypePrompt = "random";

        std::cout << "You have selected the " << quoteOptionTypePrompt << " type for the quote of the day" << std::endl;
        std::cout << "Would you like to keep this selection? <Y or N> ";
        std::getline(std::cin, userInput);
        while (userInput != "n" && userInput != "N" && userInput != "no" && userInput != "No" && userInput != "NO")
        {
            std::cout << "Wrong answer\n\nWould you like to keep this selection? <Y or N> ";
            std::getline(std::cin, userInput);
        }
            userDone = true;

    }

    conf["quoteOptionType"] = quoteOptionType;
}

std::string QuoteOfTheDayWidget::getConfiguration() {
    return quoteOptionType;
}

nlohmann::json QuoteOfTheDayWidget::refreshData() {

    // refreshData is only called from the parent class's refresh method
    // refresh is responsible for checking to see if it is time to refresh
    // this method always refreshes and does not check the time interval
    // the method does update its refreshed time

    JSONHTTPReq req = JSONHTTPReq();

    if (conf["quoteOptionType"] != "")
        baseUrl = "http://quotes.rest/qod.json?category=" + quoteOptionType;
    else
        baseUrl = "http://quotes.rest/qod.json";

    std::string url = baseUrl;
    req.setUrl(url); //sets the URL
    req.send();      //sends out the URL and receives the response from the website

    nlohmann::json json = req.getJSONResponse(); //gets the response that was returned

    //extracts the desired data from the json that the website returned and puts it back into a simpler json
    nlohmann::json transformedJson = transformResponse(json);

    return transformedJson; //returns the json to the calling function

}

std::string QuoteOfTheDayWidget::QuoteOfTheDayWidget::getQuote() {
    return quote;
}

void QuoteOfTheDayWidget::setQuote(std::string quoteStr) {
    quote = quoteStr;
}

std::string QuoteOfTheDayWidget::getAuthor() {
    return author;
}

void QuoteOfTheDayWidget::setAuthor(std::string authorStr) {
    author = authorStr;
}

std::string QuoteOfTheDayWidget::getQuoteTypeOption() {
    return quoteOptionType;
}

void QuoteOfTheDayWidget::setQuoteTypeOption(std::string quoteTypeOptionStr) {
    quoteOptionType = quoteTypeOptionStr;
}

nlohmann::json QuoteOfTheDayWidget::transformResponse(nlohmann::json response) {

    quote = response["contents"]["quotes"][0]["quote"];  //accesses the quote and author from json that the website returned
    author = response["contents"]["quotes"][0]["author"];

    nlohmann::json data = { //creates the new, simpler json data containing only the desired information
            {"quote",  quote},
            {"author", author}
    };

    return data;
};
