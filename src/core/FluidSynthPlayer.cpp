#include "FluidSynthPlayer.hpp"
#include "CbreakMode.hpp"
#include <iostream>
#include <csignal>

extern volatile sig_atomic_t isPlaying;

FluidSynthPlayer::FluidSynthPlayer()
{
    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);
    player = new_fluid_player(synth);

    // Disable all logging from FluidSynth
    fluid_set_log_function(FLUID_PANIC, NULL, NULL);
    fluid_set_log_function(FLUID_ERR, NULL, NULL);
    fluid_set_log_function(FLUID_WARN, NULL, NULL);
    fluid_set_log_function(FLUID_INFO, NULL, NULL);
    fluid_set_log_function(FLUID_DBG, NULL, NULL);

    // Change the period size to 2048 to avoid
    // XRUNs on low-end hardware.
    fluid_settings_setint(settings, "audio.period-size", 2048);
}

FluidSynthPlayer::~FluidSynthPlayer()
{
    if (!isPlaying)
    {
        std::cerr << "Signal handled\n";
    }

    // Remove all FluidSynth elements
    // from the heap.
    delete_fluid_audio_driver(audioDriver);
    delete_fluid_player(player);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
}

bool FluidSynthPlayer::togglePause(const std::string &midiFile)
{
    // Check to see if player is not playing anything in the first place.
    // if (fluid_player_get_status(player) != FLUID_PLAYER_PLAYING)
    // {
    //     std::cerr << "Player is not currently playing\n";
    //     return false;
    // }

    // Pause music.
    if (!isPaused)
    {
        fluid_player_stop(player);
        isPaused = true;
    }
    else
    {
        fluid_player_play(player);
        isPaused = false;
    }

    return true;
}

bool FluidSynthPlayer::play(const std::string &midiFile, const std::string &soundfontFile)
{
    // Check if the user entered a valid
    // MIDI file and SoundFont file
    if (!fluid_is_soundfont(soundfontFile.c_str()))
    {
        std::cerr << "Could not load SoundFont file\n";
        return false;
    }

    if (!fluid_is_midifile(midiFile.c_str()))
    {
        std::cerr << "Could not load MIDI file\n";
        return false;
    }

    // Load SoundFont and MIDI file
    fluid_synth_sfload(synth, soundfontFile.c_str(), 1);
    fluid_player_add(player, midiFile.c_str());

    // Start the synthesizer.
    audioDriver = new_fluid_audio_driver(settings, synth);

    // Play the MIDI file.
    if (fluid_player_play(player) != FLUID_OK)
    {
        std::cerr << "Could not start MIDI playback\n";
        return false;
    }

    // Indicate that the player is not paused
    isPaused = false;

    // Keep playing until we reach EOF
    // or we CTRL+C
    while (isPlaying)
    {
        if (!isPlaying && fluid_player_get_status(player) == FLUID_PLAYER_DONE)
        {
            break;
        }


    char ch = getCharFromKeyboard();
    if (ch == 'p') togglePause(midiFile);


    }

    // Stop immediately if we CTRL+C
    if (!isPlaying)
    {
        fluid_player_stop(player);
    }

    return true;
}
