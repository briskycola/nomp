#include "MPVPlayer.hpp"
#include <iostream>
#include <array>
#include <csignal>

extern volatile sig_atomic_t isPlaying;

MPVPlayer::MPVPlayer()
{
    // Create a new mpv instance.
    mpvHandle = mpv_create();
    if (!mpvHandle)
    {
        throw std::runtime_error("Failed to create mpv instance");
    }

    // Initalize the new mpv instance.
    if (mpv_initialize(mpvHandle) < 0)
    {
        throw std::runtime_error("Failed to intialize mpv instance");
    }

    // Custom options for mpv instance.

    // Disable video features
    mpv_set_property_string(mpvHandle, "vo", "none");
}

MPVPlayer::~MPVPlayer()
{
    if (!isPlaying)
    {
        //std::cerr << "Signal handled\n";
    }

    if (mpvHandle)
    {
        mpv_terminate_destroy(mpvHandle);
    }
    mpvHandle = nullptr;
}

bool MPVPlayer::play(const std::string &filename)
{
    // This array represents the command sent
    // to the mpv instance along with it's arguments.
    //
    // In this case, we are going to load an
    // audio file and specify the file name.
    std::array<const char*, 3> mpvCommand = {"loadfile", filename.c_str(), nullptr};

    // Used to store mpv events (EOF, interrupt, etc).
    mpv_event *event;

    // Send the command to the mpv instance.
    if (mpv_command(mpvHandle, mpvCommand.data()) < 0)
    {
        std::cerr << "Failed to load audio file\n";
        return false;
    }

    // Keep playing until we reach EOF
    // or we CTRL+C.
    //std::cout << "Music player started successfully\n";
    while (isPlaying)
    {
        event = mpv_wait_event(mpvHandle, 0);
        if (event && event->event_id == MPV_EVENT_END_FILE)
        {
           break;
        }
    }
    return true;
}
