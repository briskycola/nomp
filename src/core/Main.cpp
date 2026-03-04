#include "nctui.hpp"
#include <thread>
#include "StartPlayer.hpp"
#include "CbreakMode.hpp"
#include <csignal>

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
    init_curses();
    std::thread dis(display);

    termios original;

    // Enable Cbreak mode.
    enableCbreakMode(original);

    // Start the music player.
    startPlayer(argc, argv);

    // Disable Cbreak mode.
    disableCbreakMode(original);
    
    dis.join();

    endwin();
    return 0;
}
