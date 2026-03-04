#include "StartPlayer.hpp"
#include "CbreakMode.hpp"
#include <csignal>

volatile sig_atomic_t isPlaying = true;

void handleSignal(int signal)
{
    isPlaying = false;
}

int main(int argc, char **argv)
{
    // Check for signals from the OS.
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);

    termios original;

    // Enable Cbreak mode.
    enableCbreakMode(original);

    // Start the music player.
    startPlayer(argc, argv);

    // Disable Cbreak mode.
    disableCbreakMode(original);
    return 0;
}
