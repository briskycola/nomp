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

//Initializing ncurses library tools

volatile sig_atomic_t isResizeNeeded = false; //window resizing

void handleSigwinch(int signal)
{
    isResizeNeeded = true;
}

void NompTUI::initCurses() //initialize everything for ncurses
{
    signal(SIGWINCH, handleSigwinch);
    //setlocale(LC_ALL, "");
    initscr(); //initializes ncurses
    start_color(); //starts color
    init_pair(HOVERING, COLOR_YELLOW, COLOR_MAGENTA); // color pair definition, change later, testing
    init_pair(NEUTRAL, COLOR_CYAN, COLOR_BLACK);
    init_pair(SELECTED, COLOR_MAGENTA, COLOR_WHITE);
    noecho(); //dont show user input
    halfdelay(1);
    curs_set(0); //gets rid of cursor

    //initialize windows here
    songList = newwin(LINES-1, (COLS/4)-1, 1, 1); //newwin(xlength (up down), ylength (left right), xpos, ypos<>);
    currPlay = newwin(4*(LINES/6),COLS/2, 1, (COLS/4));
    controlBar= newwin((LINES/3)+2,(COLS/2)-2, 2*(LINES/3)-1, (COLS/4)+1);
    queueList = newwin(LINES-1,COLS/4, 1, 3*(COLS/4));
    
    keypad(stdscr, TRUE); //allows keypad
    keypad(currPlay, TRUE);
    keypad(songList, TRUE);
    keypad(controlBar, TRUE);
    keypad(queueList, TRUE);

    windows.push_back(songList);
    windows.push_back(controlBar);
    windows.push_back(queueList);
    
    currWin = windows.begin(); //iterator at start of vector    
}

//initializing MPV and Fluidsynth players
void NompTUI::initPlayer() //initialize player references and variables
{
    getSongs = std::make_unique<GetSongs>();
    mpvPlayer = std::make_unique<MPVPlayer>();
    fluidSynthPlayer = std::make_unique<FluidSynthPlayer>();
    files = getSongs->getSongFilePaths(); // All files in song folder
    currSong = files.begin(); //iterator for visual files
    //queue = getSongs->getSongFilePaths(); //queue of next songs //Breaks for some reason?
    queue.reserve(20);
    queue = {}; 
    queueTop = queue.begin(); // iterator that points to the actual current song
    listorqueue = false;
}

void NompTUI::nextInQueue()
{
    if(mpvPlayer->isIdle() && listorqueue) //if the mpvplayer is idle, and the last song was played from the queue
    {
        wclear(queueList);
        displayQueue();
        queueTop++;
        if(queueTop == queue.end()) queueTop--;
        play(*queueTop, "SoundFonts/HeartGold SoulSilver (WIP).sf2");
    }
}

void NompTUI::play(const std::string &filename, const std::string &soundfont)
{
    // Stop MPV and FluidSynth players (if they are playing)
    mpvPlayer->stop();
    fluidSynthPlayer->stop();

    // Check if the audio file is a MIDI file.
    //
    // If it's a MIDI file, activate FluidSynth.
    // If it's a regular audio file, activate MPV.
    //
    // MPV will handle the rest of the error checking internally.
    if (fluidSynthPlayer->isValidFile(filename, soundfont))
    {
        fluidSynthPlayer->play(filename, soundfont);
        isFluidSynth = true;
    }

    else
    {
        mpvPlayer->play(filename);
        displayMetadataOnTUI();
        isFluidSynth = false;
    }
}


void NompTUI::displaySongs() //display contents of song list to songList
{
    // for each song in files
    for(long unsigned int fi = 0; fi<files.size(); fi++){
        // print just the name on each descending
        // converting from path > string > const char*
        filenamestr = files[fi].filename().string();
        if(*currSong==files[fi]){
            wattron(songList, COLOR_PAIR(NEUTRAL));
            mvwprintw(songList, 2*fi+5, 2, "%s", filenamestr.c_str());
            wattroff(songList, COLOR_PAIR(NEUTRAL));
        }
        else mvwprintw(songList,2*fi+5, 2, "%s", filenamestr.c_str());
        wrefresh(*currWin);
    }
        
}

void NompTUI::displayQueue() //display contents of song list to songList
{
    //TODO: keeps highlighting multiple, previous fix would only highlight the first one
    // for each song in files
    for(long unsigned int q = 0; q<queue.size(); q++){
        // print just the name on each descending
        // converting from path > string > const char*
        queuefstr = queue[q].filename().string();
        if(*queueTop==queue[q]){ 
            wattron(queueList, COLOR_PAIR(HOVERING));
            mvwprintw(queueList,2*q+5,2,"%s",queuefstr.c_str());
            wattroff(queueList, COLOR_PAIR(HOVERING));
        }
        else mvwprintw(queueList,2*q+5,2,"%s",queuefstr.c_str());
        
        wrefresh(queueList);
    }
        
}

void NompTUI::deleteWindows() //delete windows
{
    for (auto a : windows) { if (a) delwin(a); }
    windows.clear();
}


void NompTUI::displayScreen() // display and refresh screen
{
    if (isResizeNeeded)
    {
        endwin();
        refresh();
        deleteWindows();
        initCurses();
        isResizeNeeded = false;
    }

    //function call to read songs off of folder/playlist here
    //display text using mvwprintw([window], x, y
    const std::string title = mpvPlayer ? mpvPlayer->getMetadata("title") : "";
    const std::string artist = mpvPlayer ? mpvPlayer->getMetadata("artist") : "";
    const std::string album = mpvPlayer ? mpvPlayer->getMetadata("album") : "";
    
    //function call to read songs off of folder/playlist here
    //display text using mvwprintw([window], x, y
    mvwprintw(songList,2,10,"Song List");
    mvwprintw(currPlay,2,(getmaxx(currPlay)/2)-9,"Currently Playing");
    mvwprintw(currPlay,4,2,"Title:  %s", title.empty() ? "N/A" : title.c_str());
    mvwprintw(currPlay,5,2,"Artist: %s", artist.empty() ? "N/A" : artist.c_str());
    mvwprintw(currPlay,6,2,"Album:  %s", album.empty() ? "N/A" : album.c_str());
    mvwprintw(controlBar,2,10,"Control Bar");
    mvwprintw(queueList,2,10,"Queue");

    wattron(currPlay,COLOR_PAIR(NEUTRAL));
    box(currPlay,0,0);
    wattroff(currPlay,COLOR_PAIR(NEUTRAL));
    wrefresh(currPlay);
    
    for (auto a : windows)
    {
        if (a == *currWin)
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
    displayQueue();
    wrefresh(songList);
    wrefresh(queueList);
}

// Anything ending in "Select" should be interpreted as "Selected" and is what happens when each window is selected after pressing Enter

// What happens when songList is selected
void NompTUI::songListSelect() 
{
    wbkgd(*currWin,COLOR_PAIR(3));
    while(userInput!=127 && userInput!=KEY_BACKSPACE && userInput!='\b')
    {
        displaySongs();
        wrefresh(*currWin);
        switch (getUserInput(*currWin))
        {
            case '\n':
            case KEY_ENTER:
                play(*currSong, "SoundFonts/HeartGold SoulSilver (WIP).sf2"); //path to file (wav, flac, mp3, etc)
                listorqueue = false;
                break;
            case KEY_DOWN:
                if(currSong!=files.end()-1) currSong++;
                continue;
            case KEY_UP:
                if(currSong!=files.begin()) currSong--;  
                continue;
            case KEY_RIGHT:
            case 'd':
                currWin++;
                wbkgd(songList,COLOR_PAIR(0));
                displayScreen();
                break;
            case 'j':
                //add to queue and play now
                queue.insert(queue.begin(), *currSong);
                play(*queueTop, "SoundFonts/HeartGold SoulSilver (WIP).sf2");
                //fluidSynthPlayer->play(*queueTop,"");
                displayQueue();
                listorqueue = true;
                break;
            case 'k':
                //play next
                if(queue.size()>0) queue.insert(queue.begin()+1, *currSong);
                else queue.insert(queue.begin(), *currSong);
                displayQueue();
                continue;
            case 'l':
                //push back
                queue.push_back(*currSong);
                displayQueue();
                continue;
            default:
                continue;
        }
        break;
    }
    wbkgd(*currWin,COLOR_PAIR(0));
}


// what happens when controlbar is selected
void NompTUI::controlBarSelect()
{
    wbkgd(*currWin,COLOR_PAIR(3));
    while(userInput!=127 && userInput!=KEY_BACKSPACE && userInput!='\b')
    {
        switch (getUserInput(*currWin))
        {
            case 'p':
                if (isFluidSynth) fluidSynthPlayer->togglePause();
                else mpvPlayer->togglePause();
            case ',':
                mpvPlayer->seek("-5");
                continue;
            case '.':
                mpvPlayer->seek("5");
                continue;
            default:
                // wrefresh(*currWin);
                continue;
        }
        break;
    }
    wbkgd(*currWin,COLOR_PAIR(0));
}

//what happens if settings is selected
void NompTUI::queueSelect()
{   
    wbkgd(*currWin,COLOR_PAIR(3));
    while(userInput!=127 && userInput!=KEY_BACKSPACE && userInput!='\b')
    {
        displayQueue();
        switch (getUserInput(*currWin))
        {
            case KEY_LEFT:
            case 'a':
                currWin--;
                wbkgd(queueList,COLOR_PAIR(0));
                displayScreen();
                break;
            case '\n':
            case KEY_ENTER:
                play(*queueTop, "SoundFonts/HeartGold SoulSilver (WIP).sf2");
                listorqueue = true;
                break;
            case 'c':
                queue.clear();
                queueTop=queue.begin();
                wclear(queueList);
                mpvPlayer->play("");
                listorqueue=false;
            default:
                continue;
        }
        break;
    }
    wbkgd(*currWin,COLOR_PAIR(0));
}

int NompTUI::getUserInput(WINDOW *win) //get user input
{
    userInput = wgetch(win);
    return userInput;
}

void NompTUI::selectWindow() // handle window selection
{
    userInput = getUserInput(*currWin);
    switch (userInput)
    {
    case KEY_LEFT:
        if(currWin!=windows.begin())
        {
            currWin--;
        };
        break;

    case KEY_RIGHT:
        if(currWin!=windows.end()-1)
        {
        currWin++;
        };
        break;
        
    case '\n': //enter pressed
        if(*currWin==songList) songListSelect(); 
        //else if(*currWin==currPlay) currPlaySelect();
        else if(*currWin==controlBar) controlBarSelect();
        else if(*currWin==queueList) queueSelect();
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
        mpvPlayer->play("");
        listorqueue = false;
        break;
    case ',':
        mpvPlayer->seek("-5");
        break;        
    case '.':
        mpvPlayer->seek("5");
        break;
    case '<':
        wclear(queueList);
        displayQueue();
        if(queueTop!=queue.begin()) queueTop--;
        play(*queueTop, "SoundFonts/HeartGold SoulSilver (WIP).sf2");
        break;
    case '>':
        wclear(queueList);
        displayQueue();
        if(queueTop!=queue.end()-1) queueTop++;
        play(*queueTop, "SoundFonts/HeartGold SoulSilver (WIP).sf2");
        break;
    default:
        break;
    }
}

void NompTUI::displayMetadataOnTUI()
{
    wclear(currPlay);
    for (int i = 0; i < 20; ++i)
    {
        const std::string title = mpvPlayer->getMetadata("title");
        const std::string artist = mpvPlayer->getMetadata("artist");
        const std::string album = mpvPlayer->getMetadata("album");

        if (!title.empty() || !artist.empty() || !album.empty())
        {
            break;
        }
    }
    displayScreen();
    return;
}
