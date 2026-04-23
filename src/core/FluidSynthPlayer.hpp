#pragma once
#include <fluidsynth.h>
#include <string>

class FluidSynthPlayer
{
    private:
        // Used to set settings for
        // audio drivers, midi drivers, etc
        fluid_settings_t *settings;

        // The main structure for rendering audio.
        // Handles SoundFonts and events from
        // the MIDI file.
        fluid_synth_t *synth;

        // Used to handle MIDI file playback
        // and sends MIDI events to the synth.
        fluid_player_t *player;

        // Used to connect the OS audio API
        // to fluidsynth.
        fluid_audio_driver_t *audioDriver;

        // Used to hold the status of the player
        fluid_player_status status;

        // Preserve SoundFont ID
        // 0 = no SoundFont loaded
        int sfid;

        // Bool variable to check if player
        // is currently paused or not.
        //fluid_player_status isPaused;
       
    public:
        FluidSynthPlayer();
        ~FluidSynthPlayer();
        bool isValidFile(const std::string &midiFile, const std::string &soundfontFile);
        bool play(const std::string &midiFile, const std::string &soundfontFile);
        bool stop();
        bool togglePause();
};
