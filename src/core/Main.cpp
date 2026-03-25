#include "StartPlayer.hpp"
#include "CbreakMode.hpp"
#include "nctui.hpp"
#include <thread>
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

    NompTUI tui;

    // Run ncurses UI in a separate thread.
    std::thread uiThread([&tui]() {
        tui.initCurses();
        while (isPlaying)
        {
            tui.displayScreen();
            tui.selectWindow();
        }
    });

    // Start the music player.
    const bool ok = startPlayer(argc, argv);

    // Stop the UI loop once the player is done.
    isPlaying = false;
    uiThread.join();

#if defined(__linux__) || defined(__APPLE__)
    // Disable Cbreak mode.
    disableCbreakMode(original);
#endif

    return ok ? 0 : 1;
}
