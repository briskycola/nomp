#include "StartPlayer.hpp"
#include "MPVPlayer.hpp"
#include "FluidSynthPlayer.hpp"
#include <iostream>
#include <memory>

bool startPlayer(int argc, char **argv)
{
    // Check if the user entered the audio file
    // or MIDI + SoundFont as an argument
    if (argc == 2)
    {
        // Create the mpv instance and
        // play the audio file.
        const std::string filename = argv[1];
        std::unique_ptr<MPVPlayer> player = std::make_unique<MPVPlayer>();
        player->play(filename);
    }

    else if (argc == 3)
    {
        // Create the FluidSynth instance and
        // play the MIDI file.
        const std::string midiFile = argv[1];
        const std::string soundfontFile = argv[2];
        std::unique_ptr<FluidSynthPlayer> player
            = std::make_unique<FluidSynthPlayer>();
        player->play(midiFile, soundfontFile);
    }

    else
    {
        std::cerr << "Usage: " << argv[0] << " <audio-file>\n";
        std::cerr << "Usage: " << argv[0] << " <midi-file> <soundfont-file>\n";
        return false;
    }
    return true;
}
