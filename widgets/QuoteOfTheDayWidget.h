
#ifndef QUOTEOFDAY_H
#define QUOTEOFDAY_H
#define QODWIDGET_NAME "QuoteOfTheDay"

#include <string>
#include "Widget.h"
#include "../include/json.hpp"

class QuoteOfTheDayWidget : public Widget {

public:

    QuoteOfTheDayWidget();
    QuoteOfTheDayWidget(std::string quoteTypeOption);
    QuoteOfTheDayWidget(nlohmann::json configJson);

    void config() override;
    std::string getConfiguration();
    nlohmann::json getConfigurationJson();
    nlohmann::json refreshData() override;
    std::string getQuote();
    std::string getAuthor();

    ~QuoteOfTheDayWidget();

private:

    std::string quote;
    std::string author;
    std::string quoteOptionType;
    std::string baseUrl;

    nlohmann::json transformResponse(nlohmann::json response);

    void setQuote(std::string quote);


    void setAuthor(std::string author);

    std::string getQuoteTypeOption();
    void setQuoteTypeOption(std::string quoteTypeOption);

};


#endif //QUOTEOFDAY_QUOTEOFDAY_H
