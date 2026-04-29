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
	      std::filesystem::path soundFontsFolder;
	  public:
	      // Constructor
	      GetSongs();

	      // Return vector full of all song files in songsFolder folder.
	      std::vector<std::filesystem::path> getFilePaths(std::filesystem::path path);
};
