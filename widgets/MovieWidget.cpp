#include "MovieWidget.h"

void MovieWidget::config()
{
    std::string buffer; //hold temp input

    std::cout << "Movie Showtimes Widget : Configure Settings" << std::endl;

    ////// api_key //////

    //std::cout << "Please enter an OnConnect API Key:" << std::endl;
    //std::getline(std::cin, buffer);
    buffer = "ssksbm4f8gr6ws6ua3vtaugf";

    while(!isAPIkeyValid(buffer))
    {
        std::cout << "Invalid API Key, please enter again:" << std::endl;
        std::getline(std::cin, buffer);
    }

    conf["api_key"] = buffer;

    ////// zip //////

    std::cout << "Please enter a 5 digit zip code:" << std::endl;
    std::getline(std::cin, buffer);
    //buffer = "32612";

    while(!Valid::isValid("zipCode", buffer))
    {
        std::cout << "Invalid ZIP code, please enter again:" << std::endl;
        std::getline(std::cin, buffer);
    }

    conf["zip"] = buffer;

    ////// numDays //////

    conf["numDays"] = "1";

    ////// DONE //////

    std::cout << "Thank you. Configuration Settings Complete!" << std::endl;
}



void MovieWidget::doRequest(const std::string& url)
{
    req.setUrl(url);
    req.send();
}


JSON MovieWidget::refreshData()
{
    JSON res;
    JSON output;

    // build request
    std::string url = baseUrl + "?";
    url += "startDate=" + Date::getISO8601("date");
    url += "&numDays=" + conf["numDays"].get<std::string>();
    url += "&zip=" + conf["zip"].get<std::string>();
    url += "&api_key=" + conf["api_key"].get<std::string>();

    // do request and return empty json if failed
    doRequest(url);
    if(req.getHTTPStatus() != 200)
        return output;

    res = req.getJSONResponse();

    // Build output //

    const std::vector<std::string> genKeys = {
            "title"
    };



    // loop through movies in JSON response
    for(const JSON& mItem : res)
    {
        JSON movie;

        for(const std::string& key : genKeys)
        {
            if(mItem.find(key) != mItem.end())
                movie[key] = mItem[key];
        }

        if(mItem.find("runTime") != mItem.end())
        {
            std::string runTime = mItem["runTime"];
            runTime = runTime.substr(2, 6);

            movie["runTime"] = runTime;
        }

        if(mItem.find("ratings") != mItem.end()
           && mItem["ratings"][0].find("code") != mItem["ratings"][0].end())
            movie["rating"] = mItem["ratings"][0]["code"];

        JSON showtimes;

        for(const JSON& showTimeItem : mItem["showtimes"])
        {
            if(showTimeItem.find("theatre") != showTimeItem.end()
               && showTimeItem["theatre"].find("name") != showTimeItem["theatre"].end())
            {
                std::string theatre = showTimeItem["theatre"]["name"];

                if(showtimes.find(theatre) == showtimes.end())
                    showtimes[theatre] = JSON();

                if(showTimeItem.find("dateTime") != showTimeItem.end())
                {
                    std::string dateTime = showTimeItem["dateTime"];
                    dateTime = dateTime.substr(11, 5);

                    showtimes[theatre].push_back(dateTime);
                }

            }
        }

        movie["showtimes"] = showtimes;

        output.push_back(movie);
    }

    return output;
}


const bool MovieWidget::isAPIkeyValid(const std::string& api_key)
{
    // build dummy request to test api key validity
    std::string url = baseUrl + "?";
    url += (std::string)"startDate=" + "2013-01-01"; //wrong date to get empty response
    url += (std::string)"&zip=" + "12345";			//dummy zipcode
    url += "&api_key=" + api_key;

    doRequest(url);

    if(req.getHTTPStatus() != 200)
        return false;

    return true;
}