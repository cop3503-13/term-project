#include "WeatherWidget.h"
#include <string>
#include "httpreq.h"
#include "jsonhttpreq.h"



/****************************
 Constructors
 ____________________________
 ****************************/

WeatherWidget::WeatherWidget(){}

/************
 * config string parameter should be the whole configuration, like this:
 * {
 *      "name": "Weather",
 *      "configuration": {
 *          "zip": #####,
 *          "refreshInterval": ###  //(in seconds)
 *      }
 * }
 * @param config
 */

WeatherWidget::WeatherWidget(std::string config)
{
    nlohmann::json configJson = nlohmann::json::parse(config);
    setZipCode(configJson["configuration"]["zip"]);
    setRefreshInterval(configJson["configuration"]["refreshInterval"]);
}

/*********
 * config parameter should be only the "configuration" section of
 * the json:
 *      { "zip": #####, "refreshInterval": ### }
 *
 * @param config
 */
WeatherWidget::WeatherWidget(nlohmann::json config)
{
    setZipCode(config["zip"]);
    setRefreshInterval(config["refreshInterval"]);
}








/****************************
 Public
 ___________________________
 ****************************/
void WeatherWidget::configure()
{
    std::string zip = "";
    std::cout << "Please configure the Weather Widget." << std::endl;
    std::cout << "What is your zipcode?" << std::endl;
    std::cout << "Zipcode: ";
    std::getline(std::cin, zip);
    while (!validZipCode(zip))
    {
        std::cout << "What is your zipcode?" << std::endl;
        std::cout << "Zipcode: ";
        std::getline(std::cin, zip);
    }

    setZipCode(zip);
}


//returns string representation of configuration,
//used to create config file
std::string WeatherWidget::getConfiguration()
{
    nlohmann::json configurationJson = getConfigurationJson();
    return configurationJson.dump(4);
}

//returns json representation of configuration,
//used to create config file
nlohmann::json WeatherWidget::getConfigurationJson()
{
    nlohmann::json configurationJson = {
            {"name", "Weather"},
            {"configuration", {
                             {"zip", getZipCode()},
                             {"refreshInterval", getRefreshInterval()}
                     }}
    };

    return configurationJson;
}

//returns string representation of weather results
//
//Uses the openweather API and then transforms into subset of information
std::string WeatherWidget::refreshData()
{
    JSONHTTPReq req = JSONHTTPReq();
    std::string url = ENDPOINT + "?zip=" + getZipCode() + ",us&appid=" + APIKEY;
    req.setUrl(url);
    req.send();
    nlohmann::json json = req.getJSONResponse();
    nlohmann::json transformedJson = transformResponse(json);
    return transformedJson.dump(4);
}








/****************************
 Private
 ___________________________
 ****************************/

//returns URL for daily weather forecast
std::string WeatherWidget::getDailyUrl(std::string zip)
{
    return ENDPOINT + "?zip=" + zip + ",us&appid=" + APIKEY;
}

//returns URL for 5 day forecast
std::string WeatherWidget::get5dayUrl(std::string zip)
{
    //todo: do this
    return "todo";
}

/************************
 *
 * @param zip
 * @return bool validity of zip code
 */
bool WeatherWidget::validZipCode(std::string zip)
{
    if (zip.length() != 5)
    {
        std::cout << "Sorry, zip codes must be 5 digits." << std::endl;
        return false;
    }
    for (int i = 0; i < zip.length(); ++i)
    {
        int charcode = zip[i] + 0;
        if (charcode < 48 || charcode > 57)
        {
            std::cout << "Sorry, zip code must contain only digits" << std::endl;
            return false;

        }
    }
    //we know the zip is now 5 digits, so let's check if it's valid
    //in the weather api
    JSONHTTPReq req = JSONHTTPReq();
    std::string url = getDailyUrl(zip);
    req.setUrl(url);
    req.send();
    nlohmann::json json = req.getJSONResponse();
    if (json["cod"] == "200" || json["cod"] == 200)
        return true;
    if (json["cod"] == "404" || json["cod"] == 404)
        std::cout << "Error: bad zip code, " << json["message"] << std::endl;
    return false;
}


std::string WeatherWidget::getZipCode()
{
    return zipcode;
}


void WeatherWidget::setZipCode(std::string zip)
{
    zipcode = zip;
}

/*****************
 * Transforms the response from the openweather api request into
 * the useful information we need, and returns it in json format
 *
 * @param response
 * @return json
 */
nlohmann::json WeatherWidget::transformResponse(nlohmann::json response)
{
    int currentTemp = response["main"]["temp"];
    currentTemp = currentTemp * 9 / 5 - 459.67;

    nlohmann::json data = {
            {"city",response["name"]},
            {"temp", currentTemp},
            {"description", response["weather"][0]["description"]}
    };

    nlohmann::json json = {
            {"name", "Weather"},
            {"data", data}
    };

    return json;
};