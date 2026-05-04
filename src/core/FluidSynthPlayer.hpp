#pragma once
#include <fluidsynth.h>
#include <filesystem>

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

        // Boolean that holds whether or
        // not reverb is being applied
        bool isReverb;
        
    public:
        FluidSynthPlayer();
        ~FluidSynthPlayer();
        bool isValidMidi(const std::filesystem::path &midiFile);
        bool isValidSoundFont(const std::filesystem::path &soundFontFile);
        bool isIdle();
        int loadSoundFont(const std::filesystem::path &soundFontFile);
        int getCurrentTick();
        int getTotalTicks();
        bool play(const std::filesystem::path &midiFile, const std::filesystem::path &soundFontFile);
        bool stop();
        void seek(double time);
        void reverb();
        bool togglePause();
};
