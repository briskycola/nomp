//IMPORTANT
//X and Y are "flipped" in terminal
//X is up and down Y is left-right
//#include "StartPlayer.hpp"
//
//       TODO:
//       Dynamic window resizing
//       Selection: Change to use directed graph to handle smooth use input
//       Add more windows
//       Use pads for screens that need to scroll
//       Iterators become invalidated if a vector is resized.
//       First song lags now, fix that
// 
#include "nctui.hpp"
#include "GetSongs.hpp"
#include <memory>
#include <ncurses.h>
#include <vector>

#if defined(__linux__) || defined(__APPLE__)
    #include <csignal>
#endif

#define HOVERING 1
#define NEUTRAL 2
#define SELECTED 3

// Used asynchronously to check if the window
// needs to be resized
#if defined(__linux__) || defined(__APPLE__)
volatile sig_atomic_t isResizeNeeded = false;

// Signal handler to handle SIGWINCH
void handleSigwinch(int signal)
{
    isResizeNeeded = true;
}
#endif

// Initialize ncurses and all windows
void NompTUI::initCurses()
{
    // Asynchronously check for SIGWINCH signal
#if defined(__linux__) || defined(__APPLE__)
    signal(SIGWINCH, handleSigwinch);
#endif

    // Set locale to UTF-8
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "C");

    // Initialize ncurses itself along with colors
    initscr();
    start_color();
    init_pair(HOVERING, COLOR_YELLOW, COLOR_MAGENTA);
    init_pair(NEUTRAL, COLOR_CYAN, COLOR_BLACK);
    init_pair(SELECTED, COLOR_MAGENTA, COLOR_WHITE);

    // Don't show user input
    noecho();

    // Set terminal to half-delay mode
    halfdelay(1);
    set_escdelay(25);

    // Gets rid of cursor
    curs_set(0);

    // Initialize windows here
    songList = newwin(LINES-1, (COLS/4)-1, 1, 1); //newwin(xlength (up down), ylength (left right), xpos, ypos<>);
    currentlyPlaying = newwin(4*(LINES/6),COLS/2, 1, (COLS/4));
    soundFontList = newwin((LINES/3)+2,(COLS/2)-2, 2*(LINES/3)-1, (COLS/4)+1);
    about = newwin((LINES/3)+2,(COLS/4), 2*(LINES/3)-1, (COLS/2));
    queueList = newwin(LINES-1,COLS/4, 1, 3*(COLS/4));

    // Allow windows to use keypad
    keypad(stdscr, TRUE);
    keypad(currentlyPlaying, TRUE);
    keypad(songList, TRUE);
    keypad(soundFontList, TRUE);
    keypad(about, TRUE);
    keypad(queueList, TRUE);
    
    windows.push_back(songList);
    windows.push_back(soundFontList);
    windows.push_back(about);
    windows.push_back(queueList);

    // Iterator at the start of vector
    currentWindow = windows.begin();
}

// Initialize MPV and Fluidsynth players
void NompTUI::initPlayer()
{
    getSongs = std::make_unique<GetSongs>();
    mpvPlayer = std::make_unique<MPVPlayer>();
    fluidSynthPlayer = std::make_unique<FluidSynthPlayer>();
    isFluidSynth = false;
}

// Initialize Paths for songs
void NompTUI::initPaths()
{
    audioFiles = getSongs->getFilePaths("NompSongs");
    soundFontFiles = getSongs->getFilePaths("SoundFonts");
    currentSong = audioFiles.begin();
    currentSoundFont = soundFontFiles.begin();
}

// Initialize Queue for songs
void NompTUI::initQueue()
{
    queue.reserve(20);
    queue = {};
    queueTop = queue.begin();
    isQueue = false;
}

void NompTUI::nextInQueue()
{
    bool isIdle = false;

    // If the next song in the queue is a MIDI file,
    // check if FluidSynth is idle. Otherwise, check if
    // MPV is idle
    if (isFluidSynth) isIdle = fluidSynthPlayer->isIdle();
    else isIdle = mpvPlayer->isIdle();

    // Check if the queue is empty
    if (queue.empty()) isQueue = false;

    // If the mpvplayer is idle, and the last song was played from the queue
    if (isIdle && isQueue)
    {
        wclear(queueList);
        displayQueue();
        queueTop++;
        if(queueTop == queue.end()) queueTop--;
        play(*queueTop, *currentSoundFont);
    }
}

void NompTUI::play(const std::filesystem::path &audioFile, const std::filesystem::path &soundFontFile)
{
    // Stop MPV and FluidSynth players (if they are playing)
    mpvPlayer->stop();
    fluidSynthPlayer->stop();

    // Clear metadata from the screen for new metadata
    wclear(currentlyPlaying);

    // Check if the audio file is a MIDI file.
    //
    // If it's a MIDI file, activate FluidSynth.
    // If it's a regular audio file, activate MPV.
    //
    // MPV will handle the rest of the error checking internally.
    if (fluidSynthPlayer->isValidMidi(audioFile))
    {
        if (fluidSynthPlayer->isValidSoundFont(soundFontFile))
        {
            fluidSynthPlayer->play(audioFile, soundFontFile);
            isFluidSynth = true;
        }
    }

    else
    {
        mpvPlayer->play(audioFile);
        displayScreen();
        isFluidSynth = false;
    }
}

// Returns int with remaining time represented as a fraction of the window size
int NompTUI::statusBar()
{
    if (isFluidSynth)
    {
        // Get current tick
        int currentTick = fluidSynthPlayer->getCurrentTick();

        // Get total ticks
        int totalTicks = fluidSynthPlayer->getTotalTicks();

        // Get current size of display window -2
        int winSize = getmaxx(currentlyPlaying)-2;

        // Check if total number of ticks and window size
        // are 0 ahead of time to avoid divide-by-zero
        // undefined behavior
        if (totalTicks == 0) return 0;
        if (winSize == 0) return 0;

        // Determine how much of the window width should be filled
        // for the progress bar based on the playback progress
        double result = (double) currentTick / (double) totalTicks * winSize;
        return (int) result;
    }

    else
    {
        // Get and convert current time from mpv to double
        std::string currentTime = mpvPlayer ? mpvPlayer->getProperty("time-pos") : "0";
        currentTime = currentTime=="" ? "0" : currentTime;

        // Get and convert remaining time from mpv to double
        std::string totalTime = mpvPlayer ? mpvPlayer->getProperty("duration") : "0";
        totalTime = totalTime == "" ? "0" : totalTime;
        
        // Get current size of display window -2
        int winSize = getmaxx(currentlyPlaying)-2;
        
        // Convert current time and total time from
        // a string to a double
        double currentTimeDouble = std::stod(currentTime);
        double totalTimeDouble = std::stod(totalTime);

        // Check if the total time and window size
        // is 0 ahead of time to avoid divide-by-zero
        // undefined behavior
        if (totalTimeDouble == 0) return 0;
        if (winSize == 0) return 0;

        // Determine how much of the window width should be filled
        // for the progress bar based on the playback progress
        double result = currentTimeDouble / totalTimeDouble * winSize;
        return result;
    }
}


void NompTUI::displaySongs() //display contents of song list to songList
{
    // Get width of the window to truncate characters properly
    int maxWidth = getmaxx(songList);
    int startX = 2;
    int properWidth = maxWidth - startX - 1;
    if (properWidth < 0) properWidth = 0;

    for (long unsigned int fi = 0; fi < audioFiles.size(); fi++)
    {
        // print just the name on each descending
        // converting from path > string > const char*
        std::string filenameString = audioFiles[fi].filename().string();

        if (*currentSong == audioFiles[fi])
        {
            wattron(songList, COLOR_PAIR(NEUTRAL));
            mvwprintw(songList, 2*fi+5, startX, "%-*.*s", properWidth-1, properWidth-1, filenameString.c_str());
            wattroff(songList, COLOR_PAIR(NEUTRAL));
        }

        else mvwprintw(songList, 2*fi+5, startX, "%-*.*s", properWidth-1, properWidth-1, filenameString.c_str());
        wrefresh(*currentWindow);
    }
}

void NompTUI::displaySoundFonts() //display contents of song list to songList
{
    // Get width of the window to truncate characters properly
    int maxWidth = getmaxx(soundFontList);
    int startX = 2;
    int properWidth = maxWidth - startX - 1;
    if (properWidth < 0) properWidth = 0;

    for (long unsigned int fi = 0; fi < soundFontFiles.size(); fi++)
    {
        // print just the name on each descending
        // converting from path > string > const char*
        std::string filenameString = soundFontFiles[fi].filename().string();

        if (*currentSoundFont == soundFontFiles[fi])
        {
            wattron(soundFontList, COLOR_PAIR(NEUTRAL));
            mvwprintw(soundFontList, 2*fi+5, startX, "%-*.*s", properWidth-1, properWidth-1, filenameString.c_str());
            wattroff(soundFontList, COLOR_PAIR(NEUTRAL));
        }

        else mvwprintw(soundFontList, 2*fi+5, startX, "%-*.*s", properWidth-1, properWidth-1, filenameString.c_str());
        wrefresh(*currentWindow);
    }
}

// Display contents of song list to songList
void NompTUI::displayQueue()
{
    // Get width of the window to truncate characters properly
    int maxWidth = getmaxx(queueList);
    int startX = 2;
    int properWidth = maxWidth - startX - 1;
    if (properWidth < 0) properWidth = 0;

    // TODO: keeps highlighting multiple, previous fix would only highlight the first one
    for (long unsigned int q = 0; q < queue.size(); q++)
    {
        // Print just the name on each descending
        // converting from path > string > const char*
        std::string queuedSongString = queue[q].filename().string();

        if (std::distance(std::begin(queue), queueTop) == (long int) q)
        { 
            wattron(queueList, COLOR_PAIR(HOVERING));
            mvwprintw(queueList, 2*q+5, startX, "%-*.*s", properWidth-1, properWidth-1, queuedSongString.c_str());
            wattroff(queueList, COLOR_PAIR(HOVERING));
        }

        else mvwprintw(queueList, 2*q+5, startX, "%-*.*s", properWidth-1, properWidth-1, queuedSongString.c_str());
        wrefresh(queueList);
    }
        
}

// Delete windows
void NompTUI::deleteWindows()
{
    for (auto a : windows) { if (a) delwin(a); }
    windows.clear();
}

// Convert seconds to corresponding minutes and seconds
std::string NompTUI::prettyPrintTime(int totalSeconds)
{
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // MM:SS Format
    std::ostringstream oss;
    // Pad seconds with a 0 in front if less than 10 (e.g. 00:04)
    oss << minutes << ":" << (seconds < 10 ? "0" : "") << seconds;
    return oss.str();
}

// Display and refresh screen
void NompTUI::displayScreen()
{
    int currentTick, totalTicks, maxColumns, statusBarTotal;
    std::string currentTimeFormatted, totalTimeFormatted;

    // Re-create the TUI to change the window size
    // when SIGWINCH is recieved
#if defined(__linux__) || defined(__APPLE__)
    if (isResizeNeeded)
    {
        endwin();
        refresh();
        deleteWindows();
        initCurses();
        isResizeNeeded = false;
    }
#endif

    // Retrieve metadata from MPV backend
    const std::string title = mpvPlayer ? mpvPlayer->getProperty("metadata/title") : "";
    const std::string artist = mpvPlayer ? mpvPlayer->getProperty("metadata/artist") : "";
    const std::string album = mpvPlayer ? mpvPlayer->getProperty("metadata/album") : "";
    const std::string date = mpvPlayer ? mpvPlayer->getProperty("metadata/date") : "";
    const std::string currentTime = mpvPlayer ? mpvPlayer->getProperty("time-pos") : "";
    const std::string totalTime = mpvPlayer ? mpvPlayer->getProperty("duration") : "";

    if (!isFluidSynth)
    {
        // Convert current and total time seconds into MM:SS format
        currentTimeFormatted = currentTime.empty() ? "N/A" : prettyPrintTime(std::stoi(currentTime));
        totalTimeFormatted = totalTime.empty() ? "N/A" : prettyPrintTime(std::stoi(totalTime));
    }

    else
    {
        currentTick = fluidSynthPlayer->getCurrentTick();
        totalTicks = fluidSynthPlayer->getTotalTicks();
    }


    // Display all content to the TUI
    int maxWidth = getmaxx(currentlyPlaying);
    mvwprintw(songList, 2, 10, "Song List");
    mvwprintw(currentlyPlaying, 2, (getmaxx(currentlyPlaying)/2)-9,"Currently Playing");

    if (!isFluidSynth)
    {
        mvwprintw(currentlyPlaying, 4, 2, "Title:          %-*.*s", maxWidth, maxWidth, title.empty() ? "N/A" : title.c_str());
        mvwprintw(currentlyPlaying, 5, 2, "Artist:         %-*.*s", maxWidth, maxWidth, artist.empty() ? "N/A" : artist.c_str());
        mvwprintw(currentlyPlaying, 6, 2, "Album:          %-*.*s", maxWidth, maxWidth, album.empty() ? "N/A" : album.c_str());
        mvwprintw(currentlyPlaying, 7, 2, "Date:           %-*.*s", maxWidth, maxWidth, date.empty() ? "N/A" : date.c_str());
        mvwprintw(currentlyPlaying, 8, 2, "Current Time:   %-*.*s", maxWidth, maxWidth, currentTimeFormatted.c_str());
        mvwprintw(currentlyPlaying, 9, 2, "Total Time:     %-*.*s", maxWidth, maxWidth, totalTimeFormatted.c_str());
    }

    else
    {
        mvwprintw(currentlyPlaying, 4, 2, "Current Tick:   %d", currentTick);
        mvwprintw(currentlyPlaying, 5, 2, "Total Ticks:    %d", totalTicks);
    }

    mvwprintw(about, 2, 10, "About NOMP");
    mvwprintw(soundFontList, 2, 10, "SoundFonts");
    mvwprintw(queueList, 2, 10, "Queue");

    wattron(currentlyPlaying, COLOR_PAIR(NEUTRAL));
    box(currentlyPlaying, 0, 0);
    wattroff(currentlyPlaying, COLOR_PAIR(NEUTRAL));
    wrefresh(currentlyPlaying);
    
    for (auto a : windows)
    {
        if (a == *currentWindow)
        {
            wattron(a,COLOR_PAIR(1));
            box(a,0,0);
            wattroff(a,COLOR_PAIR(1));
        }
        else
        {
            wattron(a,COLOR_PAIR(2));
            box(a,0,0);
            wattroff(a,COLOR_PAIR(2));
        }
        wrefresh(a);
    }

    maxColumns = getmaxx(currentlyPlaying) - 2;
    statusBarTotal = statusBar();

    for (int a = 0; a < maxColumns; a++)
    {
        mvwaddch(currentlyPlaying, 16, a+1, '.');
    }
    
    for (int a = 0; a <= statusBarTotal; a++)
    {
        wattron(currentlyPlaying, COLOR_PAIR(SELECTED));
        mvwaddch(currentlyPlaying, 16, a+1, '-');
        wattroff(currentlyPlaying, COLOR_PAIR(SELECTED));
    }
    
    displaySongs();
    displaySoundFonts();
    displayQueue();
    wrefresh(songList);
    wrefresh(queueList);
}

void NompTUI::displayAbout()
{
    clear();
    mvprintw(1,0," _____                                                            _____ ");
    mvprintw(2,0,"( ___ )                                                          ( ___ )");
    mvprintw(3,0," |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   | ");
    mvprintw(4,0," |   |  ██████   █████    ███████    ██████   ██████ ███████████  |   | ");
    mvprintw(5,0," |   | ░░██████ ░░███   ███░░░░░███ ░░██████ ██████ ░░███░░░░░███ |   | ");
    mvprintw(6,0," |   |  ░███░███ ░███  ███     ░░███ ░███░█████░███  ░███    ░███ |   | ");
    mvprintw(7,0," |   |  ░███░░███░███ ░███      ░███ ░███░░███ ░███  ░██████████  |   | ");
    mvprintw(8,0," |   |  ░███ ░░██████ ░███      ░███ ░███ ░░░  ░███  ░███░░░░░░   |   | ");
    mvprintw(9,0," |   |  ░███  ░░█████ ░░███     ███  ░███      ░███  ░███         |   | ");
    mvprintw(10,0," |   |  █████  ░░█████ ░░░███████░   █████     █████ █████        |   | ");
    mvprintw(11,0," |   | ░░░░░    ░░░░░    ░░░░░░░    ░░░░░     ░░░░░ ░░░░░         |   | ");
    mvprintw(12,0," |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___| ");
    mvprintw(13,0,"(_____)                                                          (_____)");
    mvprintw(15,0, "Control Cheat Sheet:                              Contributers:");
    mvprintw(17,0, "P - Pause song                                    Santiago Torres");
    mvprintw(18,0, "C - Clear queue                                   Riley White");
    mvprintw(19,0, "J - Add song to top of queue and play             Brandon Mercado");
    mvprintw(20,0, "K - Play next                                     Jeremiah Galloway");
    mvprintw(21,0, "L - Add to end of queue                           Joseph Garcia");
    mvprintw(22,0, ". - 5 seconds forward");
    mvprintw(23,0, ", - 5 seconds back");
    mvprintw(24,0, "> - Next song");
    mvprintw(25,0, "< - Previous song");
    mvprintw(27,0, "Press [e] to exit...");
    refresh();
}

// Anything ending in "Select" should be interpreted as "Selected" and is what happens when each window is selected after pressing Enter

// What happens when songList is selected
void NompTUI::songListSelect() 
{
    switch (getUserInput(*currentWindow))
    {
        case '\n': case KEY_ENTER:
            if (audioFiles.empty()) break;
            play(*currentSong, *currentSoundFont);
            isQueue = false;
            break;
            
        case KEY_DOWN:
            if (currentSong != audioFiles.end()-1) currentSong++;
            break;

        case KEY_UP:
            if (currentSong != audioFiles.begin()) currentSong--;  
            break;

        case KEY_RIGHT: case 'd':
            currentWindow++;
            displayScreen();
            break;

        case 'r':
            if (isFluidSynth) fluidSynthPlayer->reverb();
            break;

        case 'p':
            if (isFluidSynth) fluidSynthPlayer->togglePause();
            else mpvPlayer->togglePause();
            break;

        case 'c':
            queue.clear();
            queueTop = queue.begin();
            wclear(queueList);
            mpvPlayer->stop();
            fluidSynthPlayer->stop();
            isQueue = false;
            isFluidSynth = false;
            displayScreen();
            break;
            
        case 'j':
            // Add to queue and play now
            if (audioFiles.empty()) break;
            if (queue.size() >= 20) break;
            queue.insert(queue.begin(), *currentSong);
            play(*queueTop, *currentSoundFont);
            displayQueue();
            isQueue = true;
            break;

        case 'k':
            // Play next
            if (audioFiles.empty()) break;
            if (queue.size() >= 20) break;
            if (queue.size() > 0) queue.insert(queue.begin()+1, *currentSong);
            else queue.insert(queue.begin(), *currentSong);
            displayQueue();
            break;

        case 'l':
            // Push back
            if (audioFiles.empty()) break;
            if (queue.size() >= 20) break;
            queue.push_back(*currentSong);
            displayQueue();
            break;
        
        case ',':
            if (isFluidSynth) fluidSynthPlayer->seek(-5.0);
            else mpvPlayer->seek("-5");
            break;

        case '.':
            if (isFluidSynth) fluidSynthPlayer->seek(5.0);
            else mpvPlayer->seek("5");
            break;

        case '<':
            wclear(queueList);
            displayQueue();
            if(queueTop != queue.begin()) queueTop--;
            play(*queueTop, *currentSoundFont);
            break;

        case '>':
            wclear(queueList);
            displayQueue();
            if(queueTop != queue.end()-1) queueTop++;
            play(*queueTop, *currentSoundFont);
            break;
                            
        default: break;
    }
}


// what happens when controlbar is selected
void NompTUI::soundFontSelect()
{
    switch (getUserInput(*currentWindow))
    {
        case '\n': case KEY_ENTER:
            if (soundFontFiles.empty()) break;
            if (fluidSynthPlayer->isValidSoundFont(*currentSoundFont))
            {
                fluidSynthPlayer->loadSoundFont(*currentSoundFont);
            }
            break;
            
        case KEY_DOWN:
            if (currentSoundFont != soundFontFiles.end()-1) currentSoundFont++;
            break;

        case KEY_UP:
            if (currentSoundFont != soundFontFiles.begin()) currentSoundFont--;  
            break;

        case KEY_LEFT: case 'a':
            currentWindow--;
            displayScreen();
            break;

        case KEY_RIGHT: case 'd':
            currentWindow++;
            displayScreen();
            break;

        case 'r':
            if (isFluidSynth) fluidSynthPlayer->reverb();
            break;

        case 'p':
            if (isFluidSynth) fluidSynthPlayer->togglePause();
            else mpvPlayer->togglePause();
            break;

        case 'c':
            queue.clear();
            queueTop = queue.begin();
            wclear(queueList);
            mpvPlayer->stop();
            fluidSynthPlayer->stop();
            isQueue = false;
            isFluidSynth = false;
            displayScreen();
            break;
            
        case ',':
            if (isFluidSynth) fluidSynthPlayer->seek(-5.0);
            else mpvPlayer->seek("-5");
            break;

        case '.':
            if (isFluidSynth) fluidSynthPlayer->seek(5.0);
            else mpvPlayer->seek("5");
            break;

        case '<':
            wclear(queueList);
            displayQueue();
            if(queueTop != queue.begin()) queueTop--;
            play(*queueTop, *currentSoundFont);
            break;

        case '>':
            wclear(queueList);
            displayQueue();
            if(queueTop != queue.end()-1) queueTop++;
            play(*queueTop, *currentSoundFont);
            break;

        default: break;
    }
}

void NompTUI::aboutSelect()
{
    switch (getUserInput(*currentWindow))
    {
        case KEY_ENTER: case '\n':
            //open contributor page + help manuali
            displayAbout();
            while(getUserInput(*currentWindow)!='e'){}
            clear();
            refresh();
            break;

        case KEY_LEFT: case 'a':
            currentWindow--;
            displayScreen();
            break;

        case KEY_RIGHT: case 'd':
            currentWindow++;
            displayScreen();
            break;

        case 'p':
            if (isFluidSynth) fluidSynthPlayer->togglePause();
            else mpvPlayer->togglePause();
            break;

        case 'c':
            queue.clear();
            queueTop = queue.begin();
            wclear(queueList);
            mpvPlayer->stop();
            fluidSynthPlayer->stop();
            isQueue = false;
            isFluidSynth = false;
            displayScreen();
            break;

        case ',':
            if (isFluidSynth) fluidSynthPlayer->seek(-5.0);
            else mpvPlayer->seek("-5");
            break;

        case '.':
            if (isFluidSynth) fluidSynthPlayer->seek(5.0);
            else mpvPlayer->seek("5");
            break;

        case '<':
            wclear(queueList);
            displayQueue();
            if(queueTop != queue.begin()) queueTop--;
            play(*queueTop, *currentSoundFont);
            break;
            
        case '>':
            wclear(queueList);
            displayQueue();
            if(queueTop != queue.end()-1) queueTop++;
            play(*queueTop, *currentSoundFont);
            break;

        default: break;
    }
}

//what happens if settings is selected
void NompTUI::queueSelect()
{   
    switch (getUserInput(*currentWindow))
    {
        case KEY_LEFT:
            currentWindow--;
            wbkgd(queueList,COLOR_PAIR(0));
            displayScreen();
            break;

        case '\n': case KEY_ENTER:
            if (queue.size() == 0) break;
            play(*queueTop, *currentSoundFont);
            isQueue = true;
            break;
            
        case 'r':
            if (isFluidSynth) fluidSynthPlayer->reverb();
            break;
            
        case 'p':
            if (isFluidSynth) fluidSynthPlayer->togglePause();
            else mpvPlayer->togglePause();
            break;

        case 'c':
            queue.clear();
            queueTop = queue.begin();
            wclear(queueList);
            mpvPlayer->stop();
            fluidSynthPlayer->stop();
            isQueue = false;
            isFluidSynth = false;
            displayScreen();
            break;
            
        case ',':
            if (isFluidSynth) fluidSynthPlayer->seek(-5.0);
            else mpvPlayer->seek("-5");
            break;
            
        case '.':
            if (isFluidSynth) fluidSynthPlayer->seek(5.0);
            else mpvPlayer->seek("5");
            break;
            
        case '<':
            wclear(queueList);
            displayQueue();
            if(queueTop != queue.begin()) queueTop--;
            play(*queueTop, *currentSoundFont);
            break;

        case '>':
            wclear(queueList);
            displayQueue();
            if(queueTop != queue.end()-1) queueTop++;
            play(*queueTop, *currentSoundFont);
            break;
        
        default: break;
    }
}

int NompTUI::getUserInput(WINDOW *win)
{
    userInput = wgetch(win);
    return userInput;
}

void NompTUI::selectWindow()
{
    if (*currentWindow == songList) songListSelect(); 
    else if (*currentWindow == soundFontList) soundFontSelect();
    else if (*currentWindow == queueList) queueSelect();
    else if (*currentWindow == about) aboutSelect();
    else {}
            
}
