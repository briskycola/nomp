#include "GetSongs.hpp"
#include <filesystem>
#include <iostream>

#if defined(__APPLE__)
    #include <sys/syslimits.h>
#endif

#include <system_error>
#include <algorithm>
#include <cctype>


// Constructor implementation
GetSongs::GetSongs()
{

    // Create NompSongs folder in directory
    songsFolder = std::filesystem::current_path() / "NompSongs";

    // Prevent exception interrupts
    std::error_code ec;

    // Create NompSongs
    std::filesystem::create_directories(songsFolder, ec);

    // Error handling
    if (ec)
    {
        std::cerr << "Failed to create NompSongs folder: "
              << ec.message() << '\n';
    }
}

// getSongFilePaths function implementation
std::vector<std::filesystem::path> GetSongs::getSongFilePaths()
{
    // Vector of file names to return
    std::vector<std::filesystem::path> files {};

    // Supported audio files
    //const std::vector<std::string> ALLOWED_EXTENSIONS = 
    //{
    //    ".mp3", ".wav", ".flac", ".aac", ".ogg",
    //    ".mid", ".midi", ".sf2", ".sf3", ".m4a", ".aif"
    //};

    // Only iterate through folder if it is NOT empty
    if (!songsFolder.empty()) 
    {

        // Any potential error codes are caught in this ec variable
        std::error_code ec;

        for (const auto& entry : std::filesystem::directory_iterator(songsFolder, ec))
        {

            // If there is an error, print iti
            if (ec)
            {
                std::cerr << "Error reading directory: " << ec.message() << "\n";
                break;
            }

            // Only read files (skip over subdirectoies)
            if (!entry.is_regular_file()) continue;
            // Break the string into the path and extension
            std::filesystem::path path = entry.path();
            std::string fileExtension = path.extension().string();

            // Ensure extensions are all lowercase (lambda function)
            std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(),
                    [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

            // Ensure audio file is within the allowed extensions
            //
            // std::find returns an iterator pointing to the location of where fileExtension
            // was found within ALLOWED_EXTENSIONS.
            // If the pointer does not equal the end of the array, that means a location has been found
            files.push_back(std::filesystem::absolute(path));
            //if (std::find(ALLOWED_EXTENSIONS.begin(), ALLOWED_EXTENSIONS.end(), fileExtension) != ALLOWED_EXTENSIONS.end())
            //{
            //    // Add file to vector
            //    // NOTE: If this doesn't work, remove absolute
            //    //files.push_back(path);
            //    files.push_back(std::filesystem::absolute(path));
            //}
        }
    }

    return files;
}
