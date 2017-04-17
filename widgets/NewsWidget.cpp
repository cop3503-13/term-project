#include "NewsWidget.h"

void NewsWidget::config()
{
    std::string buffer, cats = "";
    List Cats;
    List listCategory = {"", "home", "world", "business", "technology", "arts"};

    std::cout << "Please select the integer(s) for the news categories that you wish to see, separated by a comma:\n"
              << "1. Home\n"
              << "2. World\n"
              << "3. Business\n"
              << "4. Technology\n"
              << "5. Arts\n";

    std::getline(std::cin, cats);

    while(!Valid::isValid("csvIntsSpc1-5",cats))
    {
        std::cout << "Please enter a valid response: " << std::endl;
        std::getline(std::cin, cats);
    }

    List output = Common::split(cats);

    for(List::iterator it = output.begin(); it != output.end(); it++)
    {
        buffer = *it;
        Cats.push_back(listCategory[std::stoi(buffer)]);
    }

    conf =
            {
                    {"category", Cats},
                    {"API_key", "49eb7ae0f2bb40e2a415fac4965db249"}
            };

}

const bool NewsWidget::doRequest(const std::string& s)
{
    req.setUrl(s);
    req.send();

    if(req.getHTTPStatus() != 200 || !req.isOk())
        return false;

    return true;
}

JSON NewsWidget::refreshData()
{
    std::string buffer;
    std::string confKey = conf["API_key"];
    std::string fullURL;
    JSON temp, snip, final;

    for(JSON::iterator it = conf["category"].begin(); it != conf["category"].end(); it++)
    {
        temp.clear();
        snip.clear();

        buffer = *it;
        fullURL = baseUrl + buffer + ".json?api-key=" + confKey;

        if (doRequest(fullURL))
        {
            temp = req.getJSONResponse();

            for(int i = 0; i < 3; i++)
            {
                snip = {
                        {"title",temp["results"][i]["title"]},
                        {"byline",temp["results"][i]["byline"]},
                        {"updated_date",temp["results"][i]["updated_date"]},
                        {"abstract",temp["results"][i]["abstract"]}
                };
                final["news"][buffer].emplace_back(snip);
            }
        }
    }

    return final;
}