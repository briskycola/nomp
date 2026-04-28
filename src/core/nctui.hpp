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
        bool isFluidSynth;
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

        std::string queuefstr;
        const char* queuefptr;

        bool listorqueue; //false = left, true = right

      public:
        std::vector<std::filesystem::path> files;
        std::vector<std::filesystem::path>::iterator currSong;

        std::vector<std::filesystem::path> queue;
        std::vector<std::filesystem::path>::iterator queueTop;
        
        int userInput;

        void initCurses();
        void nextInQueue();
        void deleteWindows();

        void displaySongs();
        void displayQueue();
        void displayScreen();
        void displayMetadataOnTUI();
        
        void songListSelect();
        void currPlaySelect();
        void controlBarSelect();
        void queueSelect();
        
        int getUserInput(WINDOW *win);
        void selectWindow();

        // Audio Playback functions for TUI
        void initPlayer();
        void play(const std::string &filename, const std::string &soundfont);
        void togglePause();
 
};
