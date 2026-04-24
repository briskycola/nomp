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
        WINDOW *queueList;

        std::unique_ptr<GetSongs> getSongs;
        std::unique_ptr<MPVPlayer> mpvPlayer;
        std::unique_ptr<FluidSynthPlayer> fluidSynthPlayer;

        std::string filenamestr;
        const char* filenameptr;

        std::string queuefstr;
        const char* queuefptr;

      public:
        std::vector<std::filesystem::path> files;
        std::vector<std::filesystem::path>::iterator currSong;

        std::vector<std::filesystem::path> queue;
        std::vector<std::filesystem::path>::iterator queueTop;
        
        int userInput;

        void initCurses();
        void initPlayer();
        void deleteWindows();

        void displaySongs();
        void displayQueue();
        void displayScreen();
        
        void songListSelect();
        void currPlaySelect();
        void controlBarSelect();
        void queueSelect();
        
        int getUserInput(WINDOW *win);
        void selectWindow();
};
