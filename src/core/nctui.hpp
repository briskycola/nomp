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
        WINDOW *settings;

        std::unique_ptr<GetSongs> getSongs;
        std::unique_ptr<MPVPlayer> mpvPlayer;
        std::unique_ptr<FluidSynthPlayer> fluidSynthPlayer;

        std::string filenamestr;

      public:
        std::vector<std::filesystem::path> files;
        std::vector<std::filesystem::path>::iterator currSong;
        int userInput;

        void initCurses();
        void deleteWindows();

        void displaySongs();
        void displayScreen();
        
        void songListSelect();
        void currPlaySelect();
        void controlBarSelect();
        void settingsSelect();
        
        int getUserInput(WINDOW *win);
        void selectWindow();

        // Audio Playback functions for TUI
        void initPlayer();
        void play(const std::string &filename, const std::string &soundfont);
        void togglePause();
 
};
