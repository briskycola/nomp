#include "GetSongs.hpp"
#include <filesystem>
#include <iostream>
//#include <algorithm>

// Constructor implementation
GetSongs::GetSongs()
{
    // Create NompSongs folder in directory
    songsFolder = std::filesystem::current_path() / "NompSongs";
    soundFontsFolder = std::filesystem::current_path() / "SoundFonts";

    // Prevent exception interrupts
    std::error_code ec;

    // Create NompSongs and SoundFonts folders
    std::filesystem::create_directories(songsFolder, ec);
    if (ec) throw std::runtime_error("Failed to create NompSongs and SoundFonts folders");

    std::filesystem::create_directories(soundFontsFolder, ec);
    if (ec) throw std::runtime_error("Failed to create NompSongs and SoundFonts folders");
}

// getSongFilePaths function implementation
std::vector<std::filesystem::path> GetSongs::getFilePaths(std::filesystem::path folder)
{
    // Vector of file names to return
    std::vector<std::filesystem::path> files {};

    // Only iterate through folder if it is NOT empty
    if (!folder.empty()) 
    {
        // Any potential error codes are caught in this ec variable
        std::error_code ec;

        for (const auto& entry : std::filesystem::directory_iterator(folder, ec))
        {
            // If there is an error, print it
            if (ec)
            {
                std::cerr << "Error reading directory: " << ec.message() << "\n";
                break;
            }

            // Only read files (skip over subdirectoies)
            if (!entry.is_regular_file()) continue;

            // Break the string into the path and extension
            std::filesystem::path path = entry.path();
            //std::string fileExtension = path.extension().string();

            // Ensure extensions are all lowercase (lambda function)
            //std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(),
            //        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

            // Ensure audio file is within the allowed extensions
            //
            // std::find returns an iterator pointing to the location of where fileExtension
            // was found within ALLOWED_EXTENSIONS.
            // If the pointer does not equal the end of the array, that means a location has been found
            files.push_back(std::filesystem::absolute(path));
        }
    }
    return files;
}
