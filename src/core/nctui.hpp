#ifndef NCTUI_HPP
#define NCTUI_HPP

#include <vector>
#include <ncurses.h>

class NompTUI
{
private:
    bool tesRunning = false;

    WINDOW *songList = nullptr;
    WINDOW *currPlay = nullptr;
    WINDOW *window3 = nullptr;
    WINDOW *window4 = nullptr;

    std::vector<WINDOW*> windows; // vector of windows for selecting current window
    std::vector<WINDOW*>::iterator currWin = windows.begin();
    int userInput = 0;

public:
    void initCurses();
    void displayScreen();
    void songListSelect(WINDOW *win);
    int getUserInput(WINDOW *win);
    void selectWindow();

    ~NompTUI();
};

#endif // NCTUI_HPP
