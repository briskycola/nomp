#include "MPVPlayer.hpp"
#include <iostream>
#include <array>

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

bool MPVPlayer::seek(const std::string time)
{
    std::array<const char*, 4> mpvCommandSeek = {"seek", time.c_str(), "relative", nullptr};
    if (mpv_command(mpvHandle, mpvCommandSeek.data()) < 0)
    {
        std::cerr << "Failed to seek mpv player\n";
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

    // Send the command to the mpv instance.
    if (mpv_command(mpvHandle, mpvCommand.data()) < 0)
    {
        std::cerr << "Failed to load audio file\n";
        return false;
    }
    
    return true;
}

std::string MPVPlayer::getMetadata(const std::string &key) const
{
    if (!mpvHandle)
    {
        return "";
    }

    const std::string propertyName = "metadata/" + key;
    char *value = mpv_get_property_string(mpvHandle, propertyName.c_str());

    if (!value)
    {
        return "";
    }

    std::string metadataValue(value);
    mpv_free(value);
    return metadataValue;
}
