#include "nctui.hpp"
<<<<<<< HEAD
=======
#include <thread>
>>>>>>> main
#include "StartPlayer.hpp"
#include <csignal>

volatile sig_atomic_t isPlaying = true;

void handleSignal(int signal)
{
    isPlaying = false;
}
<<<<<<< HEAD

=======
>>>>>>> main
void display(){
    while(isPlaying){
        displayScreen();
        selectWindow();
    }
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
    
    init_curses(); 

    // Enable Cbreak mode.

    std::thread dis(display);
    
    // Disable Cbreak mode.
=======
    init_curses();
    std::thread dis(display);

#if defined(__linux__) || defined(__APPLE__)
    termios original;

    // Enable Cbreak mode.
    enableCbreakMode(original);
#endif

    // Start the music player.
    startPlayer(argc, argv);

#if defined(__linux__) || defined(__APPLE__)
    // Disable Cbreak mode.
    disableCbreakMode(original);
    
>>>>>>> main
    dis.join();

    endwin();
<<<<<<< HEAD
=======
#endif

>>>>>>> main
    return 0;
}
