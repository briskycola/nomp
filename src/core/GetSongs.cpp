#include "GetSongs.hpp"
#include <iostream>
#include <cstdlib>
#include <system_error>
#include <algorithm>
#include <cctype>


// Constructor implementation
GetSongs::GetSongs()
{
	
    // If home directory is found and valid
    if (const char* HOME = std::getenv("HOME"))
    {
        
	// Get absolute path of folder with songs for nomp
	// Folder is to be called `NompSongs`
        songsFolder = std::filesystem::path(HOME) / "NompSongs";

	// Prevent exception interrupts
	std::error_code ec;

	// Create NompSongs if it doesn't exist
	// -- if it *does* exist, this function does nothing.
	std::filesystem::create_directories(songsFolder, ec);

	// Error handling
	if (ec)
	{
	    std::cerr << "Failed to create NompSongs folder: "
		      << ec.message() << '\n';
	}

    }

    // Off-chance that HOME varaible has not been set
    else
    {
	// Possible on linux if HOME variable has not been set (rare, but safe.)
        std::cerr << "HOME environment variable is not set.\n";
    }
}
