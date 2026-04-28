#pragma once
#include <mpv/client.h>
#include <string>

class MPVPlayer
{
    private:
        // mpvHandle represents the mpv audio player itself.
        //
        // It handles audio playback, commands, properties,
        // and events.
        mpv_handle *mpvHandle;

        // Flag to handle whether the mpv player
        // is paused or not.
        //
        // Normally this would be a bool, but
        // mpv treats flags as an int, so
        // we will use an int.
        bool isPaused;
        bool coreIdle;
    public:
        MPVPlayer();
        ~MPVPlayer();
        bool play(const std::string &filename);
        bool stop();
        bool togglePause();
        bool seek(const std::string time);
        bool isIdle();
};
