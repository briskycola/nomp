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
#include <csignal>

#define HOVERING 1
#define NEUTRAL 2
#define SELECTED 3

// Used asynchronously to check if the window
// needs to be resized
volatile sig_atomic_t isResizeNeeded = false;

// Signal handler to handle SIGWINCH
void handleSigwinch(int signal)
{
    isResizeNeeded = true;
}

// Initialize ncurses and all windows
void NompTUI::initCurses()
{
    // Asynchronously check for SIGWINCH signal
    signal(SIGWINCH, handleSigwinch);

    // Set locale to UTF-8
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC,"C");

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
    queueList = newwin(LINES-1,COLS/4, 1, 3*(COLS/4));

    // Allow windows to use keypad
    keypad(stdscr, TRUE);
    keypad(currentlyPlaying, TRUE);
    keypad(songList, TRUE);
    keypad(soundFontList, TRUE);
    keypad(queueList, TRUE);

    windows.push_back(songList);
    windows.push_back(soundFontList);
    windows.push_back(queueList);

    // Iterator at the start of vector
    currentWindow = windows.begin();
}

// Initializing MPV and Fluidsynth players
void NompTUI::initPlayer()
{
    getSongs = std::make_unique<GetSongs>();
    mpvPlayer = std::make_unique<MPVPlayer>();
    fluidSynthPlayer = std::make_unique<FluidSynthPlayer>();
    audioFiles = getSongs->getFilePaths("NompSongs");
    soundFontfiles = getSongs->getFilePaths("SoundFonts");
    currentSong = audioFiles.begin(); //iterator for visual files
    currentSoundFont = soundFontfiles.begin();
    //queue = getSongs->getSongFilePaths(); //queue of next songs //Breaks for some reason?
    queue.reserve(20);
    queue = {}; 
    queueTop = queue.begin(); // iterator that points to the actual current song
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

void NompTUI::play(const std::string &audioFile, const std::string &soundFontFile)
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

    for (long unsigned int fi = 0; fi < soundFontfiles.size(); fi++)
    {
        // print just the name on each descending
        // converting from path > string > const char*
        std::string filenameString = soundFontfiles[fi].filename().string();

        if (*currentSoundFont == soundFontfiles[fi])
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

// Display and refresh screen
void NompTUI::displayScreen()
{
    // Re-create the TUI to change the window size
    // when SIGWINCH is recieved
    if (isResizeNeeded)
    {
        endwin();
        refresh();
        deleteWindows();
        initCurses();
        isResizeNeeded = false;
    }

    // Retrieve metadata from MPV backend
    const std::string title = mpvPlayer ? mpvPlayer->getProperty("metadata/title") : "";
    const std::string artist = mpvPlayer ? mpvPlayer->getProperty("metadata/artist") : "";
    const std::string album = mpvPlayer ? mpvPlayer->getProperty("metadata/album") : "";
    const std::string date = mpvPlayer ? mpvPlayer->getProperty("metadata/date") : "";
    const std::string currentTime = mpvPlayer ? mpvPlayer->getProperty("time-pos") : "";
    const std::string totalTime = mpvPlayer ? mpvPlayer->getProperty("duration") : "";

    // Display all content to the TUI
    int maxWidth = getmaxx(currentlyPlaying);
    mvwprintw(songList, 2, 10, "Song List");
    mvwprintw(currentlyPlaying, 2, (getmaxx(currentlyPlaying)/2)-9,"Currently Playing");
    mvwprintw(currentlyPlaying, 4, 2, "Title:          %-*.*s", maxWidth, maxWidth, title.empty() ? "N/A" : title.c_str());
    mvwprintw(currentlyPlaying, 5, 2, "Artist:         %-*.*s", maxWidth, maxWidth, artist.empty() ? "N/A" : artist.c_str());
    mvwprintw(currentlyPlaying, 6, 2, "Album:          %-*.*s", maxWidth, maxWidth, album.empty() ? "N/A" : album.c_str());
    mvwprintw(currentlyPlaying, 7, 2, "Date:           %-*.*s", maxWidth, maxWidth, date.empty() ? "N/A" : date.c_str());
    mvwprintw(currentlyPlaying, 8, 2, "Current Time:   %-*.*s", maxWidth, maxWidth, currentTime.empty() ? "N/A" : currentTime.c_str());
    mvwprintw(currentlyPlaying, 9, 2, "Total Time:     %-*.*s", maxWidth, maxWidth, totalTime.empty() ? "N/A" : totalTime.c_str());
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
    
    displaySongs();
    displaySoundFonts();
    displayQueue();
    wrefresh(songList);
    wrefresh(queueList);
}

// Anything ending in "Select" should be interpreted as "Selected" and is what happens when each window is selected after pressing Enter

// What happens when songList is selected
void NompTUI::songListSelect() 
{
    wbkgd(*currentWindow,COLOR_PAIR(3));
    while(userInput != 127 && userInput != KEY_BACKSPACE && userInput != '\b')
    {
        displaySongs();
        wrefresh(*currentWindow);
        switch (getUserInput(*currentWindow))
        {
            case '\n': case KEY_ENTER:
                if (audioFiles.empty()) break;
                play(*currentSong, *currentSoundFont);
                isQueue = false;
                break;
                
            case KEY_DOWN:
                if (currentSong != audioFiles.end()-1) currentSong++;
                continue;

            case KEY_UP:
                if (currentSong != audioFiles.begin()) currentSong--;  
                continue;

            case KEY_RIGHT: case 'd':
                currentWindow++;
                wbkgd(songList, COLOR_PAIR(0));
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
                continue;

            case 'l':
                // Push back
                if (audioFiles.empty()) break;
                if (queue.size() >= 20) break;
                queue.push_back(*currentSong);
                displayQueue();
                continue;
            
            case ',':
                if (isFluidSynth) fluidSynthPlayer->seek(-5.0);
                else mpvPlayer->seek("-5");
                continue;

            case '.':
                if (isFluidSynth) fluidSynthPlayer->seek(5.0);
                else mpvPlayer->seek("5");
                continue;
            
            default: continue;
        }
        break;
    }
    wbkgd(*currentWindow, COLOR_PAIR(0));
}


// what happens when controlbar is selected
void NompTUI::soundFontSelect()
{
    wbkgd(*currentWindow, COLOR_PAIR(3));
    while(userInput != 127 && userInput != KEY_BACKSPACE && userInput!='\b')
    {
        displaySoundFonts();
        wrefresh(*currentWindow);
        switch (getUserInput(*currentWindow))
        {
            case '\n': case KEY_ENTER:
                if (soundFontfiles.empty()) break;
                if (fluidSynthPlayer->isValidSoundFont(*currentSoundFont))
                {
                    fluidSynthPlayer->loadSoundFont(*currentSoundFont);
                }
                break;
                
            case KEY_DOWN:
                if (currentSoundFont != soundFontfiles.end()-1) currentSoundFont++;
                continue;

            case KEY_UP:
                if (currentSoundFont != soundFontfiles.begin()) currentSoundFont--;  
                continue;

            case KEY_LEFT:
                currentWindow--;
                wbkgd(soundFontList, COLOR_PAIR(0));
                displayScreen();
                break;

            case KEY_RIGHT:
                currentWindow++;
                wbkgd(soundFontList, COLOR_PAIR(0));
                displayScreen();
                break;

            case 'd':
                wbkgd(soundFontList, COLOR_PAIR(0));
                displayScreen();
                break;

            case 'p':
                if (isFluidSynth) fluidSynthPlayer->togglePause();
                else mpvPlayer->togglePause();

            case ',':
                if (isFluidSynth) fluidSynthPlayer->seek(-5.0);
                else mpvPlayer->seek("-5");
                continue;

            case '.':
                if (isFluidSynth) fluidSynthPlayer->seek(5.0);
                else mpvPlayer->seek("5");
                continue;

            default: continue;
        }
        break;
    }
    wbkgd(*currentWindow, COLOR_PAIR(0));
}

//what happens if settings is selected
void NompTUI::queueSelect()
{   
    wbkgd(*currentWindow,COLOR_PAIR(3));
    while (userInput != 127 && userInput != KEY_BACKSPACE && userInput != '\b')
    {
        displayQueue();
        switch (getUserInput(*currentWindow))
        {
            case KEY_LEFT:
                currentWindow--;
                wbkgd(queueList,COLOR_PAIR(0));
                displayScreen();
                break;

            case '\n': case KEY_ENTER:
                play(*queueTop, *currentSoundFont);
                isQueue = true;
                break;

            case 'c':
                queue.clear();
                queueTop = queue.begin();
                wclear(queueList);
                displayScreen();
                mpvPlayer->stop();
                fluidSynthPlayer->stop();
                isQueue = false;

            case ',':
                if (isFluidSynth) fluidSynthPlayer->seek(-5.0);
                else mpvPlayer->seek("-5");
                continue;
                
            case '.':
                if (isFluidSynth) fluidSynthPlayer->seek(5.0);
                else mpvPlayer->seek("5");
                continue;
            
            default: continue;
        }
        break;
    }
    wbkgd(*currentWindow, COLOR_PAIR(0));
}

int NompTUI::getUserInput(WINDOW *win)
{
    userInput = wgetch(win);
    return userInput;
}

void NompTUI::selectWindow()
{
    userInput = getUserInput(*currentWindow);
    switch (userInput)
    {
        case KEY_LEFT:
            if (currentWindow != windows.begin()) currentWindow--;
            break;

        case KEY_RIGHT:
            if (currentWindow!=windows.end()-1) currentWindow++;
            break;
            
        case '\n': // Enter pressed
            if (*currentWindow == songList) songListSelect(); 
            else if (*currentWindow == soundFontList) soundFontSelect();
            else if (*currentWindow == queueList) queueSelect();
            else {}
            break;
            
        case 'p':
            if (isFluidSynth) fluidSynthPlayer->togglePause();
            else mpvPlayer->togglePause();
            break;

        case 'c':
            queue.clear();
            queueTop=queue.begin();
            wclear(queueList);
            mpvPlayer->stop();
            fluidSynthPlayer->stop();
            isQueue = false;
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
