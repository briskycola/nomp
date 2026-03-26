#include "nctui.hpp"
#include <thread>
#include "StartPlayer.hpp"
#include <csignal>
#include "CbreakMode.hpp"

volatile sig_atomic_t isPlaying = true;

void handleSignal(int signal)
{
    isPlaying = false;
}
void display(){
    while(isPlaying){
        displayScreen();
        selectWindow();
    }
}

int main(int argc, char **argv)
{
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
    
    init_curses(); 
    std::thread dis(display);

#if defined(__linux__) || defined(__APPLE__)
    termios original;

    // Enable Cbreak mode.
    enableCbreakMode(original);
#endif

    // Start the music player.
    // startPlayer(argc, argv);

#if defined(__linux__) || defined(__APPLE__)
    // Disable Cbreak mode.
    disableCbreakMode(original);
    
    dis.join();

    endwin();
#endif

    return 0;
}
