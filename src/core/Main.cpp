#include "nctui.hpp"
<<<<<<< HEAD
#include "MPVPlayer.hpp"
#include "FluidSynthPlayer.hpp"
#include <thread>
#include <iostream>
=======
>>>>>>> Reddd
#include "StartPlayer.hpp"
#include <csignal>
#include <thread>

volatile sig_atomic_t isPlaying = true;

void handleSignal(int signal)
{
    isPlaying = false;
}

<<<<<<< HEAD
void curses(){
    init_curses();
    while(TRUE){
=======
void display(){
    while(isPlaying){
>>>>>>> Reddd
        displayScreen();
        selectWindow();
    }
}

<<<<<<< HEAD
int main(int argc, char **argv)
=======
int main(int argc, char** argv)
>>>>>>> Reddd
{
    //initialize ncurses library and windows
    std::thread cur(curses);
    // Check for signals from the OS.
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
    
    // Check if the user entered the audio file
    // or MIDI + SoundFont as an argument
    if (argc == 2)
    {
        // Create the mpv instance and
        // play the audio file.
        const std::string filename = argv[1];
        std::unique_ptr<MPVPlayer> player = std::make_unique<MPVPlayer>();
        player->play(filename);
    }

<<<<<<< HEAD
    else if (argc == 3)
    {
        // Create the FluidSynth instance and
        // play the MIDI file.
        const std::string midiFile = argv[1];
        const std::string soundfontFile = argv[2];
        std::unique_ptr<FluidSynthPlayer> player
            = std::make_unique<FluidSynthPlayer>();
        player->play(midiFile, soundfontFile);
    }

    else
    {
        std::cerr << "Usage: " << argv[0] << " <audio-file>\n";
        std::cerr << "Usage: " << argv[0] << " <midi-file> <soundfont-file>\n";
        return 1;
    }
=======
    init_curses();
    std::thread dis(display);

>>>>>>> Reddd
    // Start the music player
    //startPlayer("/Users/rileywhite/Downloads/Rosalina_Observatory_2.mp3");
    dis.join();
    endwin();
    return 0;
}
