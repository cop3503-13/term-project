
#include "Mirror.h"
#include "widgets/WeatherWidget.h"
#include "widgets/MovieWidget.h"
#include "widgets/StockWidget.h"
#include "widgets/NewsWidget.h"
#include "widgets/QuoteOfTheDayWidget.h"
#include "widgets/SportsWidget.h"
#include <limits>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "include/common.h"

/****************
 * Static Constants
 */

const std::string Mirror::CONFIG_FILENAME = "mirror_config.json";

/******************
 * Constructors
 *****************/
Mirror::Mirror()
{
    configure();
};

Mirror::Mirror(std::string configFileName)
{
    nlohmann::json conf;
    bool validFile;
    std::ifstream str(configFileName);
    try
    {
        validFile = true;
        str >> conf;
        config = conf;
        data["name"] = config["name"];

        if (numOfConfiguredWidgets() > 0)
        {
            for (nlohmann::json& existingWidgetConf : config["widgets"])
            {
                Widget* widget;
                std::string name = existingWidgetConf["name"].get<std::string>();
                if (name == "Weather")
                {
                    widget = new WeatherWidget(existingWidgetConf["configuration"]);
                    selectedWidgets.push_back(widget);
                }
                if (name == "News")
                {
                    widget = new NewsWidget(existingWidgetConf["configuration"]);
                    selectedWidgets.push_back(widget);
                }
                if (name == "Sports")
                {
                    widget = new SportsWidget(existingWidgetConf["configuration"]);
                    selectedWidgets.push_back(widget);
                }
                if (name == "Stock")
                {
                    widget = new StockWidget(existingWidgetConf["configuration"]);
                    selectedWidgets.push_back(widget);
                }
                if (name == "Movie")
                {
                    widget = new MovieWidget(existingWidgetConf["configuration"]);
                    selectedWidgets.push_back(widget);
                }
                if (name == "QuoteOfTheDay")
                {
                    widget = new QuoteOfTheDayWidget(existingWidgetConf["configuration"]);
                    selectedWidgets.push_back(widget);
                }
                if (Common::contains(allWidgets, name))
                {
                    nlohmann::json widgetConfig = {
                            {"name", name },
                            {"configuration", widget->getConfJSON()}
                    };
                    updateConfigWidget(widgetConfig);
                    publishConfig();
                }

            }
        }
        else
        {
            configure();
        }

    }
    catch (nlohmann::detail::parse_error& e) { validFile = false; }
    catch (nlohmann::detail::type_error& e)  { validFile = false; }

    if (!validFile)
    {
        selectedWidgets.clear();
        config.clear();
        data.clear();
        configure();
    }


};

Mirror::~Mirror()
{};

/******************
 * Public
 *****************/

void Mirror::run()
{
    int running = 0;
    while (!exit)
    {
        if (!keyboard_hit())
        {
            if (running != 2){
                for(int i = 0; i < 100; ++i)
                    std::cout << " \n";
                std::cout << "Running mirror... press enter to make changes or exit\n\n";
                fflush(stdout);  //flush sdout so keyboard hit will work correctly
                running++;
            }

            for(size_t i = 0; i < selectedWidgets.size(); ++i)
            {
                Widget* widget = selectedWidgets[i];
                refreshWidget(widget);
            }
        }
        else
        {
            running = 0;
            clearCin();
            configure();
        }
    }

}

void Mirror::refreshWidget(Widget* widget, bool run)
{
    std::string refreshed = "";
    bool validWidget;
    try
    {
        refreshed = widget->refresh();
        validWidget = true;
    }
    catch (nlohmann::json::out_of_range& e){ validWidget = false; }
    catch (nlohmann::detail::type_error& e){ validWidget = false; }

    if (!validWidget)
    {
        std::cout << "\n\n\nInvalid configuration detected, please reconfigure.\n";
        std::cout << "Configuring " + widget->getName() + " widget...\n\n";
        widget->config();
        refreshed = widget->refresh();
        nlohmann::json widgetConfig = {
                {"name", name },
                {"configuration", widget->getConfJSON()}
        };
        updateConfigWidget(widgetConfig);
        publishConfig();
        std::cout << "\nReconfiguration of " + widget->getName() + " widget complete, refreshing data and running mirror...\n\n";
    }

    if (refreshed != "") {
        nlohmann::json data_json = {{"name", widget->getName()},
                                    {"data", nlohmann::json::parse(refreshed)}};
        updateDataWidget(data_json);

        if (run)
            openBrowser(publishData());
        else
            publishData();
    }
    else
    {
        if (run && !webfile_open)
            openBrowser(publishData());
    }
}


void Mirror::configure()
{
    bool hasName;
    try { hasName = config["name"].get<std::string>() != "";  }
    catch (nlohmann::json::out_of_range& e){ hasName = false; }
    catch (nlohmann::detail::type_error& e){ hasName = false; }

    if (hasName)
    {
        std::cout << "Hello " + name + ", welcome to your Magic Mirror. \n";
    }
    else
    {
        std::cout << "Hello, welcome to the Magic Mirror. \n";
        changeName();

    }

    int choice;
    bool moveOn = false;

    while (!moveOn)
    {
        choice = displayMainOptions();

        switch (choice)
        {
            case 1:
                addWidget();
                publishConfig();
                break;
            case 2:
                changeName();
                publishConfig();
                break;
            case 3:
                listChosenWidgets();
                break;
            case 4:
                removeWidget();
                break;
            case 5:
                moveOn = true;
                break;
            case 6:
                exitMirror();
                moveOn = true;
                break;
            default:
                break;
        }
    }


}

void Mirror::configure(std::string configFileName)
{
    //get the file
    //extract the widgets from the file
    //call config(json) on each widget
}


/******************
 * Private
 *****************/

int Mirror::displayMainOptions()
{
    std::string message = "What would you like to do? \n";
    message += "\t 1: Add a widget to your mirror \n";
    message += "\t 2: Change name\n";
    int numOfWidgets = numOfConfiguredWidgets();
    if (numOfWidgets > 0)
    {
        message += "\t 3: List your chosen widgets\n";
        message += "\t 4: Remove a chosen widget\n\n";
        message += "\t 5: Run the Mirror\n";
    }
    message += "\t 6: Exit\n";


    std::cout << "Hi " + data["name"].get<std::string>() << std::endl << message;

    double dchoice;
    int choice;
    std::cin >> dchoice;
    choice = (int)dchoice;
    //if there are 0 widgets then choice is only 1 2 or 6
    bool invalidChoice = numOfWidgets > 0 ? (choice < 1 || choice > 6) : !(choice == 1 || choice == 2 || choice == 6);

    while (std::cin.fail()
           || (!std::cin.eof() && std::cin.peek() != 10)
           || choice != dchoice
           || invalidChoice )
    {
        std::cout << "Error you must make a valid choice\n";
        std::cout << message;
        clearCin();

        std::cin >> dchoice;
        choice = (int)dchoice;
    }

    std::cout << std::endl;
    return choice;
}

void Mirror::removeWidget()
{
    std::vector<std::string> chosenWidgets = getChosenWidgets();

    if (chosenWidgets.size() < 1)
    {
        std::cout << "There are no widgets to remove, please choose another option." << std::endl << std::endl;
    }
    else
    {
        std::string choice;
        std::string cancelMsg = "Type \"cancel\" to go back\n\n";
        std::string choicesMsg = "Which widget would you like to remove from your Magic Mirror?\n\t" + Common::join(chosenWidgets, "\n\t");

        std::cout << choicesMsg << std::endl << cancelMsg;

        std::cin >> choice;
        clearCin();

        while(!Common::contains(chosenWidgets, choice))
        {
            std::cout << "Invalid widget choice, please try again" << std::endl;
            std::cout << choicesMsg << std::endl << cancelMsg;
            std::cin >> choice;
            clearCin();
        }
        if (choice != "cancel")
            removeWidget(choice);
    }
}

void Mirror::removeWidget(std::string widgetName)
{
    for (size_t i = 0; i < config["widgets"].size(); ++i)
    {
        if (config["widgets"][i]["name"].get<std::string>() == widgetName)
            config["widgets"].erase(i);
    }

    for (size_t i = 0; i < data["widgets"].size(); ++i)
    {
        if (data["widgets"][i]["name"].get<std::string>() == widgetName)
            data["widgets"].erase(i);
    }
    publishData();

    for (size_t i = 0; i < selectedWidgets.size(); ++i)
    {
        Widget* w = selectedWidgets[i];
        w->setLastRefreshed(0);
        if (w->getName() == widgetName)
            selectedWidgets.erase(selectedWidgets.begin() + i);
    }

    std::cout << "\nRemaining widgets: \n";
    for (auto w : selectedWidgets)
        std::cout << "\t" + w->getName() << std::endl;

    publishConfig();
}

void Mirror::addWidget()
{

    std::vector<std::string> availableWidgets = getAvailableWidgets();
    if (availableWidgets.size() < 1)
    {
        std::cout << "You have configured all available widgets, please choose another option." << std::endl << std::endl;
    }
    else
    {
        std::string choice;
        std::string cancelMsg = "Type \"cancel\" to go back\n\n";
        std::string choicesMsg = "Which widget would you like to add from the following list?\n\t" + Common::join(availableWidgets, "\n\t");

        std::cout << choicesMsg << std::endl << cancelMsg;

        std::cin >> choice;
        clearCin();
        while (!Common::contains(availableWidgets, choice) && choice != "cancel")
        {
            if (widgetIsConfigured(choice))
            {
                std::cout << "This widget is already configured, if you want to re-configure, please remove it first." <<std::endl;
            }
            else
            {
                std::cout << "Invalid widget choice, please try again" << std::endl;
            }
            std::cout << choicesMsg << std::endl << cancelMsg;
            std::cin >> choice;
            clearCin();
        }
        if (choice != "cancel")
            addWidget(choice);
    }
}


/**********************************
 * Calls the configure method of the selected widget
 * and then adds it to the widgets array
 * @param widgetName
 */
void Mirror::addWidget(std::string widgetName)
{
    Widget* widget;
    if (widgetName == "News")
    {
        widget = new NewsWidget();
    }
    if (widgetName == "Weather")
    {
        widget = new WeatherWidget();
    }
    if (widgetName == "Sports")
    {
        widget = new SportsWidget();
    }
    else if (widgetName == "Stock")
    {
        widget = new StockWidget();
    }
    else if (widgetName == "Movie")
    {
        widget = new MovieWidget();
    }
    else if (widgetName == "QuoteOfTheDay")
    {
        widget = new QuoteOfTheDayWidget();
    }

    selectedWidgets.push_back(widget);
    nlohmann::json configuration = {{"name", widget->getName()}, {"configuration", widget->getConfJSON()}};

    updateConfigWidget(configuration);
    publishConfig();
    refreshWidget(widget, false);
    publishData();
}




void Mirror::updateDataWidget(nlohmann::json widgetData)
{
    bool found = false;

    if (data["widgets"].size() > 0)
    {
        for (nlohmann::json widget : data["widgets"])
        {

            if (widget["name"].get<std::string>() == widgetData["name"].get<std::string>())
            {
                found = true;
                widget = widgetData;
            }
        }
    }


    if (!found)
        data["widgets"].push_back(widgetData);
}

void Mirror::updateConfigWidget(nlohmann::json widgetConfig)
{
    bool found = false;

    if (config["widgets"].size() > 0)
    {
        for (nlohmann::json& exist : config["widgets"])
        {
            std::string existing_name = exist["name"].get<std::string>();
            std::string name_to_add = widgetConfig["name"].get<std::string>();

            if (existing_name == name_to_add)
            {
                found = true;
                exist = widgetConfig;
            }
        }
    }


    if (!found)
        config["widgets"].push_back(widgetConfig);

}

void Mirror::listChosenWidgets()
{
    std::cout << "Widgets chosen for " << data["name"] << ":\n\t";
    std::cout << Common::join(getChosenWidgets(), "\n\t");
    std::cout << std::endl << std::endl;
}

std::vector<std::string> Mirror::getChosenWidgets()
{
    std::vector<std::string> chosenWidgets;
    for (auto& chosenWidget : config["widgets"])
    {
        chosenWidgets.push_back(chosenWidget["name"].get<std::string>());
    }

    return chosenWidgets;
}

std::vector<std::string> Mirror::getAvailableWidgets()
{
    std::vector<std::string> availableWidgets = {};

    for(std::string widgetName : allWidgets)
    {
        std::vector<std::string> chosenWidgets = getChosenWidgets();
        if (std::find(chosenWidgets.begin(), chosenWidgets.end(), widgetName) == chosenWidgets.end())
            availableWidgets.push_back(widgetName);
    }

    return availableWidgets;
}

int Mirror::numOfConfiguredWidgets()
{
    try
    {
        config.at("widgets");
    }
    catch (nlohmann::json::out_of_range& e)
    {
        return 0;
    }

    return config["widgets"].size();
}

bool Mirror::widgetIsConfigured(std::string widgetName)
{
    std::vector<std::string> chosenWidgets = getChosenWidgets();
    if (std::find(chosenWidgets.begin(), chosenWidgets.end(), widgetName) != chosenWidgets.end())
        return true;
    return false;
}




void Mirror::changeName()
{
    std::cout << std::endl << "What is your name?" << std::endl << "Name: ";
    std::cin >> name;
    config["name"] = name;
    data["name"] = name;
}

void Mirror::clearCin()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Mirror::exitMirror()
{
    exit = true;
}

/*****************************************
 * This uses the getData() return to
 * write the html file that will be displayed
 * on the system
 *
 */
std::string Mirror::publishData()
{
    const char* begin_html = "<!doclabel html>\n"
            "<html ng-app=\"App\" >\n"
            "<head>\n"
            "    <meta charset=\"utf-8\">\n"
            "    <title>Magic Mirror</title>\n"
            "    <script src=\"https://ajax.googleapis.com/ajax/libs/angularjs/1.2.1/angular.js\"></script>\n"
            "    <script>\n"
            "        (function(){\n"
            "            data = ";

    const char* end_html = "})();\n"
            "    </script>\n"
            "    <script label=\"text/javascript\">\n"
            "        (function(){\n"
            "            var App = angular.module('App', []);\n"
            "            App.controller('MirrorCtrl', function($scope, $interval) {\n"
            "                $scope.name = data[\"name\"];\n"
            "                $scope.widgets = data[\"widgets\"];\n"
            "                $scope.date = function() {return new Date() };\n"
            "                $interval(function(){\n"
            "                    document.location.reload()\n"
            "                }, 1000);\n"
            "            });\n"
            "        })()\n"
            "    </script>\n"
            "    <style label=\"text/css\">\n"
            "        body{\n"
            "            font-family: Helvetica, Arial, sans-serif;\n"
            "            background-color: black;\n"
            "            color: white;\n"
            "        }\n"
            "        div > div{\n"
            "            margin-left: 10px;\n"
            "            margin-top: 0px;\n"
            "            padding-top: 0px;\n"
            "        }\n"
            "        div > span {\n"
            "            margin-right: 10px;\n"
            "        }\n"
            "        .widget{\n"
            "            margin: 20px;\n"
            "            width: 35%;\n"
            "            max-width: 35%;\n"
            "            min-height: 250px;\n"
            "            color: white;\n"
            "            float: left;\n"
            "        }\n"
            "        .widget:nth-child(even){\n"
            "            float: right;\n"
            "        }\n"
            "        h1,h2,h3,h4,h5,h6{\n"
            "            margin-bottom: 2px;\n"
            "            margin-top: 2px;\n"
            "        }\n"
            "        .author {\n"
            "            float: right;\n"
            "        }\n"
            "        .news-author {\n"
            "            margin-left: 10px;\n"
            "        }\n"
            "        .theater-info{\n"
            "            margin-left: 25px;\n"
            "        }\n"
            "        .section{\n"
            "            margin-top: 10px;\n"
            "            margin-bottom: 5px;\n"
            "        }\n"
            "        .news-type{\n"
            "            text-transform: capitalize;\n"
            "        }\n"
            "        .top{\n"
            "            width: 100%;\n"
            "            height: 50px;\n"
            "            margin-bottom: 20px;\n"
            "            padding-left: 20px;\n"
            "        }\n"
            "\n"
            "        .top .greeting{ float: left;}\n"
            "        .top .datetime{\n"
            "            padding-right: 20px;\n"
            "            float: right;\n"
            "        }\n"
            "        .widgets-container{\n"
            "            width: 100%;\n"
            "            height: 100%;\n"
            "        }\n"
            "        .widget:after{\n"
            "            content: \"\";\n"
            "            display: table;\n"
            "            clear: both;\n"
            "        }\n"
            "\n"
            "    </style>\n"
            "</head>\n"
            "<body ng-controller=\"MirrorCtrl\">\n"
            "<div class=\"top\">\n"
            "    <h1 class=\"greeting\">Hello {{name}}</h1>\n"
            "    <script> date = new Date()</script>\n"
            "    <div class=\"datetime\">\n"
            "        <h1>{{ date() | date:''}}</h1>\n"
            "        <h2>{{ date() | date:'h:mm a' }}</h2>\n"
            "    </div>\n"
            "</div>\n"
            "<div style=\"clear:both;\"></div>\n"
            "<div class=\"widgets-container\">\n"
            "    <div class=\"widget\" ng-repeat=\"widget in widgets\">\n"
            "\n"
            "        <!---->\n"
            "        <!--WEATHER-->\n"
            "        <!---->\n"
            "        <h2 ng-if=\"widget.name == 'Weather'\">Weather</h2>\n"
            "        <div ng-if=\"widget.name == 'Weather'\">\n"
            "            <div>\n"
            "                <h3 ng-bind=\"widget.data.city\"></h3>\n"
            "                <div>Current conditions: {{widget.data.description}} - {{widget.data.temp}}&deg; F</div>\n"
            "            </div>\n"
            "        </div>\n"
            "        <!---->\n"
            "        <!--QUOTE-->\n"
            "        <!---->\n"
            "\n"
            "        <h2 ng-if=\"widget.name == 'QuoteOfTheDay'\">Quote of the day</h2>\n"
            "        <div ng-if=\"widget.name == 'QuoteOfTheDay'\">\n"
            "            <div>\n"
            "                <blockquote>\n"
            "                    <div><em>{{widget.data.quote}}</em></div>\n"
            "                    <small class=\"author\">-{{widget.data.author}}</small>\n"
            "                </blockquote>\n"
            "            </div>\n"
            "        </div>\n"
            "        <!---->\n"
            "        <!--STOCKS-->\n"
            "        <!---->\n"
            "        <h2 ng-if=\"widget.name == 'Stock'\">Stock price</h2>\n"
            "        <div ng-if=\"widget.name == 'Stock'\">\n"
            "            <h3>{{widget.data.symbol}}</h3>\n"
            "            <em>Prices as of {{widget.data.date}}</em>        <dl>\n"
            "            <dt>Opening Price</dt>\n"
            "            <dd>${{ widget.data[\"Opening Price\"] }}</dd>\n"
            "            <dt>Low Price</dt>\n"
            "            <dd>${{ widget.data[\"Opening Price\"] }}</dd>\n"
            "            <dt>High Price</dt>\n"
            "            <dd>${{ widget.data[\"Opening Price\"] }}</dd>\n"
            "            <dt>Closing Price</dt>\n"
            "            <dd>${{ widget.data[\"Opening Price\"] }}</dd>\n"
            "        </dl>\n"
            "        </div>\n"
            "\n"
            "        <!---->\n"
            "        <!--Sports-->\n"
            "        <!---->\n"
            "\n"
            "        <h2 ng-if=\"widget.name == 'Sports'\">Sports TV Listings</h2>\n"
            "        <div ng-if=\"widget.name == 'Sports'\">\n"
            "            <div class=\"section\" ng-repeat=\"event in widget.data\">\n"
            "                <h3 ng-bind=\"event.title\"></h3>\n"
            "                <h4 ng-bind=\"event.eventTitle\"></h4>\n"
            "                <div ng-if=\"!event.channels.empty()\">Channels: {{event.channels.join(\", \")}}</div>\n"
            "            </div>\n"
            "        </div>\n"
            "\n"
            "\n"
            "        <!---->\n"
            "        <!--News-->\n"
            "        <!---->\n"
            "\n"
            "        <h2 ng-if=\"widget.name == 'News'\">News</h2>\n"
            "        <div ng-if=\"widget.name == 'News'\">\n"
            "            <div class=\"section\" ng-repeat=\"(newsType, headlines) in widget.data.news\">\n"
            "                <h3 class=\"news-type\" ng-bind=\"newsType\"></h3>\n"
            "                <div ng-repeat=\"headline in headlines\">\n"
            "                    <h4 ng-bind=\"headline.title\"></h4>\n"
            "                    <small class=\"news-author\">{{headline.byline}}</small>\n"
            "                    <blockquote><div><em>{{headline.abstract}}</em></div></blockquote>\n"
            "                </div>\n"
            "            </div>\n"
            "        </div>\n"
            "\n"
            "\n"
            "        <!---->\n"
            "        <!--MOVIE-->\n"
            "        <!---->\n"
            "\n"
            "        <h2 ng-if=\"widget.name == 'Movie'\">Movie Listings</h2>\n"
            "        <div ng-if=\"widget.name == 'Movie'\">\n"
            "            <div class=\"section\" ng-repeat=\"movie in widget.data\">\n"
            "                <h3 ng-bind=\"movie.title\"></h3>\n"
            "                <div class=\"movie-info\">\n"
            "                    <span ng-if=\"movie.rating\">Rated: {{movie.rating}}</span>\n"
            "                    <span ng-if=\"movie.runTime\">Runtime: {{movie.runTime}}</span>\n"
            "                </div>\n"
            "                <div class=\"theater-info\" ng-repeat=\"(theater, showtimes) in movie.showtimes\">\n"
            "                    <h4>{{theater}}</h4>\n"
            "                    <div ng-if=\"!showtimes.empty()\">Showtimes: {{showtimes.join(\", \")}}</div>\n"
            "                </div>\n"
            "            </div>\n"
            "        </div>\n"
            "\n"
            "    </div>\n"
            "</div>\n"
            "\n"
            "<h2>{{data.foo}}</h2>\n"
            "</body>\n"
            "</html>";

    std::ofstream html_file;
    std::string filename = data["name"].get<std::string>() + "_mirror.html";
    html_file.open(filename);
    html_file << begin_html;
    html_file << data.dump(4);
    html_file << end_html;
    html_file.close();
    return filename;
}

void Mirror::openBrowser(const std::string& filename)
{
    if (!webfile_open)
    {
        std::string command = "xdg-open " + filename + " &";
        system(command.c_str());
        webfile_open = true;
        std::cout << "\nOpening browser... Please wait \n\n\n";
        fflush(stdout); //flush stdout so keyboard hit will work
        sleep(3);
    }
}

void Mirror::publishConfig()
{
    std::ofstream config_file;
    config_file.open(Mirror::CONFIG_FILENAME);
    config_file << config.dump(4);
    config_file.close();
}