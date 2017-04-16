
#include "Mirror.h"
#include "widgets/WeatherWidget.h"
#include <limits>

/******************
 * Constructors
 *****************/
Mirror::Mirror()
{
    configure();
};

Mirror::Mirror(std::string configFileName)
{

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
//                    data["widgets"][json["name"].get<std::string>()] = json;
                    updateDataWidget(json);
                    std::cout << "refreshed " + std::to_string(widget->getRefreshInterval()) << std::endl;
                    std::cout << data.dump(4);
                    //update the data file
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
    std::vector<nlohmann::json> configurationJsons;

    std::cout << "Hello, welcome to the Magic Mirror. \n";
    changeName();

    int choice;
    bool moveOn = false;

    while (!moveOn)
    {
        choice = displayMainOptions();

        switch (choice)
        {
            case 1:
                addWidget();
                break;
            case 2:
                changeName();
                break;
            case 3:
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
    //call configure(json) on each widget
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
    if (widgetName == "Weather")
    {
        Widget* widget = new WeatherWidget();
        widget->configure();
        selectedWidgets.push_back(widget);
    }
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

}


/*****************************************
 * This uses the getData() return to
 * write the html file that will be displayed
 * on the system
 *
 */
void Mirror::publishData()
{

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