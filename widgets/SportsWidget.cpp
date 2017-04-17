#include "SportsWidget.h"


void SportsWidget::config()
{
    std::string buffer; //hold temp input

    std::cout << "********** Sports Events Airings Widget : Configure Settings **********" << std::endl;


    ////// api_key //////

    std::cout << "\nPlease enter an OnConnect API Key:";
    std::getline(std::cin, buffer);

    while(buffer.length() != 24 || !fetchSportsIds(buffer))
    {
        std::cout << "Invalid API Key, please enter again: ";
        std::getline(std::cin, buffer);
    }

    conf["api_key"] = buffer;


    ////// sportsId //////

    List sportsList;  //used for validation of options
    size_t column = 0;
    size_t numberOfColumns = 4;

    std::cout << "\t\t\t\t\t********** List of available Sports **********\n" << std::endl;

    for(size_t i = 0; i < numberOfColumns; i++)
    {
        std::cout << std::setw(4) << std::right << "ID" << " ";
        std::cout << std::setw(30) << std::left << "SPORT";
    }

    std::cout << std::endl << std::endl;

    for(const JSON& sportItem : req.getJSONResponse())
    {
        if(sportItem.find("sportsId") != sportItem.end()
           && sportItem.find("sportsName") != sportItem.end())
        {
            sportsList.push_back(sportItem["sportsId"].get<std::string>());

            std::cout << std::setw(4) << std::right << sportItem["sportsId"].get<std::string>() << " ";
            std::cout << std::setw(30) << std::left << sportItem["sportsName"].get<std::string>();

            if(!(++column % numberOfColumns))
                std::cout << std::endl;
        }
    }

    std::cout << "\nEnter a comma separated list of Sport IDs: ";
    std::getline(std::cin, buffer);

    while(!Valid::isValid("csvIntsSpc", buffer)  //check if valid csv
          || !Common::contains(sportsList, Common::split(buffer, true), true)) // check if selection is subset of sportsList
    {
        std::cout << "Invalid list of sports. Please try again:";
        std::getline(std::cin, buffer);
    }

    //sanitize spaces (this trims every value)
    conf["sportsId"] = Common::join(Common::split(buffer, true));



    ////// country //////

    conf["country"] = "USA"; //hardcoded for now


    ////// postalCode //////

    std::cout << "Enter a 5 digit zip code: ";
    std::getline(std::cin, buffer);

    while(!Valid::isValid("zipCode", buffer))
    {
        std::cout << "Invalid ZIP code, please enter again: ";
        std::getline(std::cin, buffer);
    }

    conf["postalCode"] = buffer;


    ////// lineupId //////

    std::cout << "\t\t\t\t\t********** List of available TV lineups in your area **********\n" << std::endl;

    if(fetchLineups())
    {
        List lineupList; //used for validation of options

        for(const JSON& lineupItem : req.getJSONResponse())
        {
            if(lineupItem.find("type") != lineupItem.end()
               && lineupItem.find("lineupId") != lineupItem.end()
               && lineupItem.find("name") != lineupItem.end())
            {
                lineupList.push_back(lineupItem["lineupId"].get<std::string>());

                std::cout << std::setw(25) << std::left << lineupItem["lineupId"].get<std::string>() << " ";
                std::cout << std::setw(15) << std::left << lineupItem["type"].get<std::string>() << " ";
                std::cout << std::setw(50) << std::left << lineupItem["name"].get<std::string>() << std::endl;
            }
        }

        std::cout << "\nChoose a lineup by entering its ID: ";
        std::getline(std::cin, buffer);

        while(!Common::contains(lineupList, buffer, true))
        {
            std::cout << "Invalid lineup ID. Please try again: ";
            std::getline(std::cin, buffer);
        }
    }

    conf["lineupId"] = buffer;
}



JSON SportsWidget::refreshData()
{
    JSON res;
    JSON output;

    if(!fetchSportsAirings())
        return output;

    res = req.getJSONResponse();

    List programKeys = {"title", "gameTime", "gameTimeZone", "eventTitle"};

    // loop through events airings in JSON response
    for(const JSON& eventItem : res)
    {
        JSON event;

        if(eventItem.find("program") != eventItem.end())
        {
            for(const std::string& key : programKeys)
            {
                if(eventItem["program"].find(key) != eventItem["program"].end())
                    event[key] = eventItem["program"][key];
            }

            if(eventItem.find("channels") != eventItem.end())
            {
                event["channels"] = eventItem["channels"];

                //success reading
                output.push_back(event);
            }
        }
    }

    return output;
}





const bool SportsWidget::doRequest(std::string& url, const std::string& api_key)
{
    if(url.back() != '?')
        url += "&";

    url += "api_key=" + api_key;
    url = baseUrl + url;

    req.setUrl(url);
    req.send();

    if(req.getHTTPStatus() != 200 || !req.isOk())
        return false;

    return true;
}

const bool SportsWidget::doRequest(std::string& url)
{
    return doRequest(url, conf["api_key"]);
}





const bool SportsWidget::fetchSportsIds(const std::string& api_key)
{
    std::string url = "sports/all?includeOrg=false";
    return doRequest(url, api_key); // passing the key cause
    // this is the first test of it
}

const bool SportsWidget::fetchSportsIds()
{
    return fetchSportsIds(conf["api_key"]);
}

const bool SportsWidget::fetchLineups()
{
    std::string url = "lineups?";
    url += "country=" + conf["country"].get<std::string>();
    url += "&postalCode=" + conf["postalCode"].get<std::string>();

    return doRequest(url);
}

const bool SportsWidget::fetchSportsAirings()
{
    std::string url = "sports/"; //comma separated list of sportsId
    url += conf["sportsId"].get<std::string>() + "/events/airings?";
    url += "startDateTime=" + Date::getISO8601("minute-resolution");
    url += "&lineupId=" + conf["lineupId"].get<std::string>();

    return doRequest(url);
}
