#pragma once
#include "MPVPlayer.hpp"
#include "GetSongs.hpp"
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

        std::unique_ptr<GetSongs> getSongs;
        std::unique_ptr<MPVPlayer> mpvPlayer;
        std::unique_ptr<FluidSynthPlayer> fluidSynthPlayer;
        
        

        std::string filenamestr;
        const char* filenameptr;

      public:
        std::vector<std::filesystem::path> files;
        std::vector<std::filesystem::path>::iterator currSong;
        int userInput;

        void initCurses();
        void initPlayer();

        void displaySongs();
        void displayScreen();
        
        void songListSelect();
        void currPlaySelect();
        void controlBarSelect();
        void settingsSelect();
        
        int getUserInput(WINDOW *win);
        void selectWindow();
};


