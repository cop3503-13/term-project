
#include "Mirror.h"
#include "widgets/WeatherWidget.h"
#include <limits>

/******************
 * Constructors
 *****************/
Mirror::Mirror()
{

};

Mirror::Mirror(std::string configFileName)
{

};

Mirror::~Mirror(){};

/******************
 * Public
 *****************/

void Mirror::run()
{
    bool exit = false;

    while (!exit)
    {
        if (!keyboard_hit())
        {
            for(size_t i = 0; i < selectedWidgets.size(); ++i)
            {
                Widget* widget = selectedWidgets[i];
                if (widget->refresh() != "")
                {
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

    //ask for persons name?

    int choice = displayMainOptions();

    switch (choice)
    {
        case 1:
            addWidget();
    }

    //switch (option){
        //if ADD
        //display widget choices
        //Widget* widget;
        //switch (chosen widget)
            // if ("weather") widget = new WeatherWidget;
        //widget.configure();
        //configurationJsons.push_back(widget->getConfigurationJson());

    //}

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
        message += "\t 2: List your chosen widgets\n";
        message += "\t 3: Remove a chosen widget\n";
        message += "\t 4: Edit a chosen widget\n";
        message += "\t 5: Exit\n";

    std::cout << message;

    double dchoice;
    int choice;
    std::cin >> dchoice;
    choice = (int)dchoice;
    while (std::cin.fail() || choice < 0 || choice > 5 || choice != dchoice)
    {
        std::cout << "Error you must make a valid choice\n";
        std::cout << message;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> choice;
    }
}

void Mirror::addWidget()
{

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