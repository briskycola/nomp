#include "mpvPlayer.hpp"
#include <iostream>
#include <csignal>
#include <memory>

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

    // Check if the user entered the
    // audio file name as an argument.
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <audio-file>\n";
        return 1;
    }

    // Create the mpv instance and
    // play the audio file.
    const std::string filename = argv[1];
    std::unique_ptr<mpvPlayer> player = std::make_unique<mpvPlayer>();
    player->play(filename);
    return 0;
}
