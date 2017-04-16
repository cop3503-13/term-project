
#include "Mirror.h"
#include "widgets/WeatherWidget.h"
#include "widgets/MovieWidget.h"
#include <limits>
#include <fstream>
#include <iostream>
#include <algorithm>

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
    std::ifstream str(configFileName);
    str >> conf;
    config = conf;
    data["name"] = config["name"];

    for (auto& existingWidgetConf : config["widgets"])
    {
        Widget* widget;
        std::string name = existingWidgetConf["name"].get<std::string>();
        if (name == "Weather")
        {
            widget = new WeatherWidget(existingWidgetConf);
            selectedWidgets.push_back(widget);
        }
        if (name == "Movie")
        {
            widget = new MovieWidget(existingWidgetConf);
            selectedWidgets.push_back(widget);
        }
    }
    if (selectedWidgets.size() < 1)
        configure();
};

Mirror::~Mirror()
{};

/******************
 * Public
 *****************/

void Mirror::run()
{
    while (!exit)
    {
        if (!keyboard_hit())
        {
            for(size_t i = 0; i < selectedWidgets.size(); ++i)
            {
                Widget* widget = selectedWidgets[i];
                std::string refreshed = widget->refresh();
                if (refreshed != "")
                {
                    nlohmann::json json = nlohmann::json::parse(refreshed);
                    updateDataWidget(json);
                    std::cout << "refreshed " + std::to_string(widget->getRefreshInterval()) << std::endl;
                    std::cout << data.dump(4);
                    publishData();
                }
            }
        }
        else
        {
            configure();
        }
    }

}


void Mirror::configure() {
    std::string name = config["name"].get<std::string>();
    if (name != "")
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
                break;
            case 5:
                break;
            case 6:
                moveOn = true;
                break;
            case 7:
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
    if (selectedWidgets.size() > 0)
    {
        message += "\t 3: List your chosen widgets\n";
        message += "\t 4: Remove a chosen widget\n";
        message += "\t 5: Edit a chosen widget\n\n";
        message += "\t 6: Run the Mirror\n";
    }
    message += "\t 7: Exit\n";


    std::cout << "Hi " + data["name"].get<std::string>() << std::endl << message;

    double dchoice;
    int choice;
    std::cin >> dchoice;
    choice = (int)dchoice;
    while (std::cin.fail() || choice < 0 || choice > 7 || choice != dchoice)
    {
        std::cout << "Error you must make a valid choice\n";
        std::cout << message;
        clearCin();

        std::cin >> dchoice;
        choice = (int)dchoice;
    }

    return choice;
}

void Mirror::addWidget()
{
    std::string choice;

    std::cout << "Which widget would you like to add from the following list?" << std::endl;
    displayAddableWidgets();

    std::cin >> choice;
    clearCin();
    while (std::find(allWidgets.begin(), allWidgets.end(), choice) == allWidgets.end())
    {
        std::cout << "Invalid widget choice, please try again" << std::endl;
        std::cout << "Which widget would you like to add from the following list?" << std::endl;
        displayAddableWidgets();

        std::cin >> choice;
        clearCin();
    }
    addWidget(choice);
}


/*****************************
 * Display a list of selected/configured widgets
 * use this when user wants to  delete or edit
 */
void Mirror::displayExistingWidgets()
{

}


/********************************
 * Display a list of widgets able to be added
 * For example, some widgets might be able to be
 * added more than once, but others only once,
 * so only display the correct widgets;
 */
void Mirror::displayAddableWidgets()
{
    std::string widgetOptions = "";

    for (auto widgetName : allWidgets)
    {
        std::cout << "\t" + widgetName << std::endl;
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
    if (widgetName == "Weather")
    {
        widget = new WeatherWidget();
    }
    else if (widgetName == "Movie")
    {
        widget = new MovieWidget();
    }

    selectedWidgets.push_back(widget);
    updateConfigWidget(widget->getConfJSON());
}



/*****************************
 * Basically, get the json results of each selectedWidget
 * and combine to make a big json object
 * foreach widget in selectedWidgets
 *                  widget.refreshData()
 *
 * @return json results of ALL selectedwidgets
 */
nlohmann::json Mirror::getData()
{
    return data;
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

    std::cout << "size: " << config["widgets"].size() << std::endl;
    for (nlohmann::json& existingWidgetConf : config["widgets"])
    {
        std::string existing_name = existingWidgetConf["name"].get<std::string>();
        std::string name_to_add = widgetConfig["name"].get<std::string>();


        if (existingWidgetConf["name"].get<std::string>() == widgetConfig["name"].get<std::string>())
        {
            found = true;
            existingWidgetConf = widgetConfig;
        }
    }

    if (!found)
        config["widgets"].push_back(widgetConfig);

    std::cout << "size now: " << config["widgets"].size() << std::endl;

}

void Mirror::listChosenWidgets()
{
    std::cout << "Widgets chosen for " << data["name"] << ":" << std::endl;
    for (auto& chosenWidget : config["widgets"])
    {
        std::cout << "\t" << chosenWidget["name"].get<std::string>() << std::endl;
    }
    std::cout << std::endl;
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.clear();
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
void Mirror::publishData()
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
            "                     document.location.reload()\n"
            "                 }, 1000);\n"
            "            });\n"
            "        })()\n"
            "    </script>\n"
            "    <style label=\"text/css\">\n"
            "        body{\n"
            "            font-family: Helvetica, Arial, sans-serif;\n"
            "            background-color: black;\n"
            "            color: white;\n"
            "            text-transform: capitalize;\n"
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
            "            height: 250px;\n"
            "            color: white;\n"
            "            background-color: #171819;\n"
            "            float: left;\n"
            "        }\n"
            "        .widget:nth-child(even){\n"
            "            float: right;\n"
            "        }\n"
            "        h1,h2,h3,h4,h5,h6{\n"
            "            margin-bottom: 2px;\n"
            "            margin-top: 2px;\n"
            "        }\n"
            "    </style>\n"
            "</head>\n"
            "<body ng-controller=\"MirrorCtrl\">\n"
            "    <h1>Hello {{name}}</h1>\n"
            "<div class=\"widget\" ng-repeat=\"widget in widgets\">\n"
            "    <h2 ng-bind=\"widget.name\"></h2>\n"
            "    <div ng-if=\"widget.name == 'Time'\">\n"
            "        <script> date = new Date()</script>\n"
            "        <div>{{ date() | date:''}}</div>\n"
            "        <div>{{ date() | date:'h:mm a' }}</div>\n"
            "    </div>\n"
            "    <div ng-if=\"widget.name == 'Weather'\">\n"
            "        <div>\n"
            "            <h3 ng-bind=\"widget.data.city\"></h3>\n"
            "            <div>{{widget.data.description}} - {{widget.data.temp}}&deg; F</div>\n"
            "        </div>\n"
            "    </div>\n"
            "    <div ng-if=\"widget.name == 'Stocks'\">\n"
            "        <div ng-repeat=\"stock in widget.data\">\n"
            "            <h3 ng-bind=\"stock.label\"></h3>\n"
            "            <div>${{stock.data}}</div>\n"
            "        </div>\n"
            "    </div>\n"
            "    <div ng-if=\"widget.name == 'Movies'\">\n"
            "        <div ng-repeat=\"location in widget.data\">\n"
            "            <h3 ng-bind=\"location.label\"></h3>\n"
            "            <div ng-repeat=\"movie in location.data\">\n"
            "                <h4>{{movie.label}}</h4>\n"
            "                <span ng-repeat=\"time in movie.data\">{{time}}</span>\n"
            "            </div>\n"
            "        </div>\n"
            "    </div>\n"
            "    <div ng-if=\"widget.name == 'News'\">\n"
            "\n"
            "    </div>\n"
            "</div>\n"
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
    if (!webfile_open)
    {
        std::string command = "xdg-open " + filename + " &";
        system(command.c_str());
        webfile_open = true;
    }
}

void Mirror::publishConfig()
{
    std::ofstream config_file;
    config_file.open(Mirror::CONFIG_FILENAME);
    config_file << config.dump(4);
    config_file.close();
}