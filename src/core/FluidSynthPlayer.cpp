#include "FluidSynthPlayer.hpp"
#include <fluidsynth/midi.h>
#include <iostream>

FluidSynthPlayer::FluidSynthPlayer()
{
    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);
    player = new_fluid_player(synth);

    audioDriver = nullptr;
    sfid = 0;
    status = (fluid_player_status) fluid_player_get_status(player);

    // Disable all logging from FluidSynth
    fluid_set_log_function(FLUID_PANIC, NULL, NULL);
    fluid_set_log_function(FLUID_ERR, NULL, NULL);
    fluid_set_log_function(FLUID_WARN, NULL, NULL);
    fluid_set_log_function(FLUID_INFO, NULL, NULL);
    fluid_set_log_function(FLUID_DBG, NULL, NULL);

    // Change the period size to 2048 to avoid
    // XRUNs on low-end hardware.
    fluid_settings_setint(settings, "audio.period-size", 512);
}

FluidSynthPlayer::~FluidSynthPlayer()
{
    if (player)
    {
        fluid_player_stop(player);
        fluid_player_join(player);
        delete_fluid_player(player);
        player = nullptr;
    }

    if (audioDriver != nullptr)
    {
        delete_fluid_audio_driver(audioDriver);
        audioDriver = nullptr;
    }

    if (synth)
    {
        delete_fluid_synth(synth);
        synth = nullptr;
    }

    if (settings)
    {
        delete_fluid_settings(settings);
        settings = nullptr;
    }
}

bool FluidSynthPlayer::isValidFile(const std::string &midiFile, const std::string &soundfontFile)
{
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
    return true;
}

bool FluidSynthPlayer::isIdle()
{
    status = (fluid_player_status) fluid_player_get_status(player);
    if (status != FLUID_PLAYER_PLAYING) return true;
    else return false;
}

bool FluidSynthPlayer::play(const std::string &midiFile, const std::string &soundfontFile)
{
    // Check if the user entered a valid
    // MIDI file and SoundFont file
    if (!isValidFile(midiFile, soundfontFile)) return false;

    if (player)
    {
        stop();
        delete_fluid_player(player);
        player = new_fluid_player(synth);
    }

    if (sfid != 0)
    {
        fluid_synth_sfunload(synth, sfid, 1);
        sfid = 0;
    }
    
    // Load SoundFont and MIDI file
    sfid = fluid_synth_sfload(synth, soundfontFile.c_str(), 1);
    fluid_player_add(player, midiFile.c_str());

    // Start the synthesizer.
    if (!audioDriver)
    {
        audioDriver = new_fluid_audio_driver(settings, synth);
    }

    // Play the MIDI file.
    if (fluid_player_play(player) != FLUID_OK)
    {
        std::cerr << "Could not start MIDI playback\n";
        return false;
    }
    status = (fluid_player_status) fluid_player_get_status(player);
    return true;
}

bool FluidSynthPlayer::stop()
{
    // Stop FluidSynth, but don't destroy the instance
    fluid_player_stop(player);
    fluid_player_join(player);
    fluid_synth_all_notes_off(synth, -1);
    fluid_synth_all_sounds_off(synth, -1);
    return true;
}

void FluidSynthPlayer::seek(double time)
{
    // To seek in FluidSynth is much different compared to
    // regular music files.
    //
    // Because MIDI music is entirely processed by the
    // computer in real-time, we need to extract the
    // tempo and division (meter) from the MIDI file itself:
    //
    // Tempo -> The speed of the song
    // Division -> The meter of the song
    int tempo = fluid_player_get_midi_tempo(player);
    int division = fluid_player_get_division(player);

    // Because FluidSynth works in ticks, we need to
    // convert the ticks to seconds
    double ticksPerSecond = (double) division * 1000000.0 / tempo;
    int deltaTicks = (int) (time * ticksPerSecond);

    // Get current tick, which represents the current point
    // in time in the music
    int currentTick = fluid_player_get_current_tick(player);
    fluid_player_seek(player, currentTick + deltaTicks);
}

bool FluidSynthPlayer::togglePause()
{
    // Pause music.
    status = (fluid_player_status) fluid_player_get_status(player);
    if (status == FLUID_PLAYER_PLAYING)
    {
        fluid_player_stop(player);
        status = (fluid_player_status) fluid_player_get_status(player);
    }
    else
    {
        fluid_player_play(player);
        status = (fluid_player_status) fluid_player_get_status(player);
    }
    return true;
}
