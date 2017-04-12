
#include "Mirror.h"
#include "WeatherWidget.h"

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
        if (!keyboard_hit)
        {
            for(Widget widget : selectedWidgets)
            {
                if (widget.refresh() != "")
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

    displayMainOptions();
    /*
     * get option:
    *      A: Add a widget
    *      E: Edit a selected widget
    *      D: Delete a configured widget
    *      S: Show configured widgets
    *      X: Exit
    */

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

void Mirror::displayMainOptions()
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

}


/**********************************
 * Calls the configure method of the selected widget
 * and then adds it to the widgets array
 * @param widgetName
 */
void Mirror::addWidget(std::string widgetName)
{

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