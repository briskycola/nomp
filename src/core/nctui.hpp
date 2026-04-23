#pragma once
#include "MPVPlayer.hpp"
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
        WINDOW *window3;
        WINDOW *window4;

        std::unique_ptr<MPVPlayer> mpvPlayer;
        std::unique_ptr<FluidSynthPlayer> fluidSynthPlayer;
      public:
        int userInput;

        void initCurses();
        void deleteWindows();
        void displayScreen();
        void songListSelect(WINDOW *win);
        int getUserInput(WINDOW *win);
        void selectWindow();

        // Audio Playback functions for TUI
        void initPlayer();
        void play(const std::string &filename, const std::string &soundfont);
        void togglePause();
 };
