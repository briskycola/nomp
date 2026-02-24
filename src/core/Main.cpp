#include "nctui.hpp"
#include "StartPlayer.hpp"
#include <csignal>
#include <thread>

volatile sig_atomic_t isPlaying = true;

void handleSignal(int signal)
{
    isPlaying = false;
}

void display(){
    while(TRUE){
        displayScreen();
        selectWindow();
    }
}

int main(int argc, char** argv)
{
    // Check for signals from the OS.
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);

    init_curses();
    std::thread dis(display);

    // Start the music player
    startPlayer("/Users/rileywhite/Downloads/Rosalina_Observatory_2.mp3");
    return 0;
}
