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
#endif

    return 0;
}
