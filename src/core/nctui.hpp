#include <vector>
#include <ncurses.h>
#ifndef nctui
#define nctui
class NompTUI
{
    private:
        
    public:
        bool tesRunning = false;
        std::vector<WINDOW*> windows;
        std::vector<WINDOW*>::iterator currWin;
        int userInput;
        
        WINDOW *songList;
        WINDOW *currPlay;
        WINDOW *window3;
        WINDOW *window4;
        
        void init_curses();
        void displayScreen();
        void songListSelect(WINDOW *win);
        int getUserInput(WINDOW *win);
        void selectWindow();
};

#endif
