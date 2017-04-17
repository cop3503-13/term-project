#ifndef TERMPROJ_MIRROR_H
#define TERMPROJ_MIRROR_H


#include <vector>
#include <string>
#include "widgets/Widget.h"
#include <iostream>
#include <stdio.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stropts.h>
#include <unistd.h>

class Mirror {
public:
    std::vector<Widget*> selectedWidgets;
    static const std::string CONFIG_FILENAME;

    Mirror();
    Mirror(std::string configFilename);
    ~Mirror();

    void run();

private:

    bool exit = false;
    bool webfile_open = false;
    std::string name;
    std::vector<std::string> const allWidgets = {"Weather", "Movie", "QuoteOfTheDay", "Stock", "News"};
    nlohmann::json config = {{"name", ""}, {"widgets", nlohmann::json::array()}};
    nlohmann::json data = {{"name", ""}, {"widgets", nlohmann::json::array()}};


    void configure();

    void configure(std::string configFileName);

    void changeName();

    /*************
     * This method will display main options:
     *       Add a widget
     *       Edit a selected widget
     *       Delete a configured widget
     *       Show configured widgets
     *       Exit
     */
    int displayMainOptions();


    /*************************
     * shows the available widgets and asks for a choice
     */
    void addWidget();

    /********************
     * shows configured widgets and asks which to remove
     */
    void removeWidget();
    void removeWidget(std::string widgetName);

    void listChosenWidgets();



    /**********************************
     * Calls the configure method of the selected widget
     * and then adds it to the widgets array
     * @param widgetName
     */
    void addWidget(std::string widgetName);





    /*****************************************
     * This uses the getData() return to
     * write the html file that will be displayed
     * on the system
     *
     */
    void publishData();

    void publishConfig();

    void updateDataWidget(nlohmann::json widgetData);
    void updateConfigWidget(nlohmann::json widgetConfig);


    /**********************
     * The following two return vectors of widget names
     * that have been chosen/configured and not yet chosen/configured
     * @return {"widgetName", "widgetName", ...}
     */
    std::vector<std::string> getChosenWidgets();
    std::vector<std::string> getAvailableWidgets();


    bool widgetIsConfigured(std::string widgetName);

    /**
     Linux (POSIX) implementation of _kbhit().
     Morgan McGuire, morgan@cs.brown.edu
     via www.flipcode.com/archives/_kbhit_for_Linux.shtml

     This returns a nonzero integer if keyboard has been hit
     USAGE:
        while (!keyboard_hit()){ do something }
     */

    int keyboard_hit() {
        //
        fflush(stdout);

        static const int STDIN = 0;
        static bool initialized = false;

        if (! initialized) {
            // Use termios to turn off line buffering
            termios term;
            tcgetattr(STDIN, &term);
            term.c_lflag &= ~ICANON;
            tcsetattr(STDIN, TCSANOW, &term);
            setbuf(stdin, NULL);
            initialized = true;
        }

        int bytesWaiting;
        ioctl(STDIN, FIONREAD, &bytesWaiting);
        return bytesWaiting;
    }

    void clearCin();

    void exitMirror();
};


#endif //TERMPROJ_MIRROR_H
