#include "MPVPlayer.hpp"
#include "FluidSynthPlayer.hpp"
#include <memory>
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
        
        void initCurses();
        void displayScreen();
        void songListSelect(WINDOW *win);
        int getUserInput(WINDOW *win);
        void selectWindow();
        std::unique_ptr<MPVPlayer> mpvPlayer;
        std::unique_ptr<FluidSynthPlayer> fsPlayer;
};

#endif
