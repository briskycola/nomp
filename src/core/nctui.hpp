#pragma once
#include "MPVPlayer.hpp"
#include "FluidSynthPlayer.hpp"
#include <vector>
#include <memory>
#include <ncurses.h>

class NompTUI
{
    private:
        std::vector<WINDOW*> windows;
        std::vector<WINDOW*>::iterator currWin;

        WINDOW *songList;
        WINDOW *currPlay;
        WINDOW *controlBar;
        WINDOW *settings;

        std::unique_ptr<MPVPlayer> mpvPlayer;
        std::unique_ptr<FluidSynthPlayer> fluidSynthPlayer;
        
      public:
        int userInput;

        void initCurses();
        void deleteWindows();
        void initPlayer();
        
        void displayScreen();
        
        void songListSelect();
        void currPlaySelect();
        void controlBarSelect();
        void settingsSelect();
        
        int getUserInput(WINDOW *win);
        void selectWindow();
};
