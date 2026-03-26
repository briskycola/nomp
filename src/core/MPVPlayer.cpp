#include "MPVPlayer.hpp"
#include "nctui.hpp"

#if defined(__linux__) || defined(__APPLE__)
#include "CbreakMode.hpp"
#endif

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

bool MPVPlayer::togglePause()
{
    // Check if we can get pause data
    // from mpv in the first place.
    if (mpv_get_property(mpvHandle, "pause", MPV_FORMAT_FLAG, &isPaused) < 0)
    {
        std::cerr << "Could not get pause data from mpv\n";
        return false;
    }

    // Determine whether the player is
    // paused or not.
    if (isPaused == 0) isPaused = 1;
    else isPaused = 0;

    // Pause/Resume the player itself.
    if (mpv_set_property(mpvHandle, "pause", MPV_FORMAT_FLAG, &isPaused) < 0)
    {
        std::cerr << "Could not pause/resume mpv instance\n";
        return false;
    }
    return true;
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

#ifdef CBREAKMODE_H
        char ch = getCharFromKeyboard();
        if (ch == 'p') togglePause();
#endif
    }
    return true;
}
