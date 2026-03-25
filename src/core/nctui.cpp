//IMPORTANT
//X and Y are "flipped" in terminal
//X is up and down Y is left-right

#include "nctui.hpp"
#include <ncurses.h>

void NompTUI::initCurses()
{
    initscr(); // initializes ncurses
    start_color(); // starts color
    init_pair(1, COLOR_BLUE, COLOR_RED); // color pair definition, change later, testing
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    noecho(); // dont show user input
    curs_set(0); // gets rid of cursor

    // initialize windows here
    // positions have to be hard-coded
    songList = newwin(30, 25, 1, 1); // newwin(xlength, ylength, xpos, ypos);
    currPlay = newwin(25, 50, 1, 30);
    window3 = newwin(25, 50, 1, 90);
    window4 = newwin(15, 20, 1, 150);

    // allow keypad input for navigation
    keypad(songList, TRUE);
    keypad(currPlay, TRUE);
    keypad(window3, TRUE);
    keypad(window4, TRUE);

    // Let wgetch() return periodically so the UI thread can stop cleanly.
    wtimeout(songList, 100);
    wtimeout(currPlay, 100);
    wtimeout(window3, 100);
    wtimeout(window4, 100);

    // set up window list for selecting current window
    windows.clear();
    windows.push_back(songList);
    windows.push_back(currPlay);
    windows.push_back(window3);
    windows.push_back(window4);

    currWin = windows.begin(); // iterator at start of vector
    tesRunning = true;
}

void NompTUI::displayScreen()
{
    // function call to read songs off of folder/playlist here
    // display text using mvwprintw([window], x, y
    for (auto a : windows)
    {
        if (a == *currWin)
        {
            wattron(a, COLOR_PAIR(1));
            box(a, 0, 0);
            wattroff(a, COLOR_PAIR(1));
        }
        else
        {
            wattron(a, COLOR_PAIR(2));
            box(a, 0, 0);
            wattroff(a, COLOR_PAIR(2));
        }
        wrefresh(a);
    }
}

// We can put a case for "ENTER" that selects the window and runs
// another method specific to each window
void NompTUI::songListSelect(WINDOW *win)
{
    wbkgd(win, COLOR_PAIR(3));
    // another array/list for scrolling through songs? esc to exit?
    wgetch(win);
    wbkgd(win, COLOR_PAIR(0));
}

int NompTUI::getUserInput(WINDOW *win)
{
    userInput = wgetch(win);
    return userInput;
}

void NompTUI::selectWindow()
{
    if (windows.empty())
    {
        return;
    }

    userInput = getUserInput(*currWin);
    switch (userInput)
    {
    case KEY_LEFT:
        if (currWin != windows.begin())
        {
            --currWin;
        }
        break;
    case KEY_RIGHT: {
        auto nextIt = currWin;
        ++nextIt;
        if (nextIt != windows.end())
        {
            currWin = nextIt;
        }
        break;
    }
    case KEY_DOWN:
        songListSelect(*currWin);
        break;
    default:
        break;
    }
}

NompTUI::~NompTUI()
{
    if (!tesRunning)
    {
        return;
    }

    for (auto w : windows)
    {
        if (w != nullptr)
        {
            delwin(w);
        }
    }
    windows.clear();

    endwin();
    tesRunning = false;
}
