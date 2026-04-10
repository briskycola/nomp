#include <iostream>
#include <thread>
#include "StartPlayer.hpp"
#include <csignal>
#include "CbreakMode.hpp"
#include "nctui.hpp"
#include <cstdlib>

volatile sig_atomic_t isPlaying = true;

NompTUI NTUI;

void handleSignal(int signal)
{
    isPlaying = false;
}

void display(){
    while(NTUI.userInput!='1'){
        NTUI.displayScreen();
        NTUI.selectWindow();
    }
}

int main(int argc, char **argv)
{
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
    
    NTUI.init_curses(); 

#if defined(__linux__) || defined(__APPLE__)
    termios original;

    // Enable Cbreak mode.
    enableCbreakMode(original);
#endif

    // Start the music player.
    // startPlayer(argc, argv);

    display();    

#if defined(__linux__) || defined(__APPLE__)
    // Disable Cbreak mode.
    disableCbreakMode(original);
    
    
#endif

   // dis.join();
    endwin();
    return 0;
}
