#include "StartPlayer.hpp"
#include "FluidSynthPlayer.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

MPVPlayer &startPlayer(int argc, const std::string &filename)
{
    // Check if the user entered the audio file
    // or MIDI + SoundFont as an argument
    // if (argc == 2)
    // {
    //     // Create the mpv istance and
    //    // play the audio fil.
    //   const std::string filename = argv[1];
    //       std::unique_ptr<MPVPlayer> player = std::make_unique<MPVPlayer>();
    //    player->play(filename);//
//
//    }
//
//    else if (argc == 3)
//    {
//        // Create the FluidSynth instance and
//        // play the MIDI file.
//        const std::string midiFile = argv[1];
//        const std::string soundfontFile = argv[2];
//        std::unique_ptr<FluidSynthPlayer> player
//            = std::make_unique<FluidSynthPlayer>();
//        player->play(midiFile, soundfontFile);
//    }
//
//    else
//    {
//        std::cerr << "Usage: " << argv[0] << " <audio-file>\n";
//        std::cerr << "Usage: " << argv[0] << " <midi-file> <soundfont-file>\n";
//        return false;
//    }
//    return true;
    std::unique_ptr<MPVPlayer> player = std::make_unique<MPVPlayer>();
    player->play(filename);
    MPVPlayer& playerptr = *player;
    return playerptr;
}
