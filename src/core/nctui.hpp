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
        // Check if we are playing audio from the
        // FluidSynth backend
        bool isFluidSynth;

        // Check if file selection was from the list or queue
        // false -> list, true -> queue
        bool isQueue;

        // Dynamic array of windows
        std::vector<WINDOW*> windows;
        std::vector<WINDOW*>::iterator currentWindow;

        // Windows themsleves
        WINDOW *songList;
        WINDOW *currentlyPlaying;
        WINDOW *soundFontList;
        WINDOW *settings;
        WINDOW *queueList;

        // Smart pointers for audio backends
        std::unique_ptr<GetSongs> getSongs;
        std::unique_ptr<MPVPlayer> mpvPlayer;
        std::unique_ptr<FluidSynthPlayer> fluidSynthPlayer;

    public:
        // Dynamic array of audio files in directory
        std::vector<std::filesystem::path> audioFiles;
        std::vector<std::filesystem::path>::iterator currentSong;

        // Dynamic array of SoundFont files in directory
        std::vector<std::filesystem::path> soundFontfiles;
        std::vector<std::filesystem::path>::iterator currentSoundFont;

        // Dynamic array of audio files in queue
        std::vector<std::filesystem::path> queue;
        std::vector<std::filesystem::path>::iterator queueTop;
        
        int userInput;

        // Initialization Functions
        void initCurses();
        void initPlayer();

        int statusBar();
        
        // Display functions for TUI
        void displaySongs();
        void displaySoundFonts();
        void displayQueue();
        void displayScreen();
        void deleteWindows();

        // Media controls
        void songListSelect();
        void currPlaySelect();
        void soundFontSelect();
        void settingSelect();
        void nextInQueue();
        void queueSelect();

        // Recieving input from user
        int getUserInput(WINDOW *win);
        void selectWindow();

        // Audio Playback functions for TUI
        void play(const std::string &audioFile, const std::string &soundFontFile);
        void togglePause();
 
};
