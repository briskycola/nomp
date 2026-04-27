#pragma once
#include <filesystem>
#include <vector>

class GetSongs
{
    private:
	
        // Hard coded file path where songs are to be placed.
	// Script will create folder for user if it does not exist.
	// Variable of File Path type.
	std::filesystem::path songsFolder;

    public:

	// Constructor
	GetSongs();
	
	// Return vector full of all song files in songsFolder folder.
	std::vector<std::filesystem::path> getSongFilePaths();
};
