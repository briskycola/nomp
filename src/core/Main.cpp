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

    // Enable Cbreak mode.

    std::thread dis(display);
    
    // Disable Cbreak mode.
    dis.join();
    endwin();
    return 0;
}
