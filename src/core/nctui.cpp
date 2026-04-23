//IMPORTANT
//X and Y are "flipped" in terminal
//X is up and down Y is left-right
//#include "StartPlayer.hpp"
#include "nctui.hpp"
#include "GetSongs.hpp"
#include <cwchar>
#include <memory>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <csignal>

#define HOVERING 1
#define NEUTRAL 2
#define SELECTED 3

//Initializing ncurses library tools

volatile sig_atomic_t isResizeNeeded = false;

void handleSigwinch(int signal)
{
    isResizeNeeded = true;
}

void NompTUI::initCurses()
{
    //setlocale(LC_ALL, "");
    initscr(); //initializes ncurses
    start_color(); //starts color
    init_pair(HOVERING, COLOR_YELLOW, COLOR_MAGENTA); // color pair definition, change later, testing
    init_pair(NEUTRAL, COLOR_CYAN, COLOR_BLACK);
    init_pair(SELECTED, COLOR_MAGENTA, COLOR_WHITE);
    noecho(); //dont show user input
    //cbreak(); //all input types
    halfdelay(2);
    curs_set(0); //gets rid of cursor

    //initialize windows here
    songList = newwin(LINES-1, (COLS/4)-1, 1, 1); //newwin(xlength (up down), ylength (left right), xpos, ypos<>);
    currPlay = newwin(4*(LINES/6),COLS/2, 1, (COLS/4));
    controlBar= newwin((LINES/3)+2,(COLS/2)-2, 2*(LINES/3)-1, (COLS/4)+1);
    settings = newwin(LINES-1,COLS/4, 1, 3*(COLS/4));
    
    keypad(stdscr, TRUE); //allows keypad
    keypad(currPlay, TRUE);
    keypad(songList, TRUE);
    keypad(controlBar, TRUE);
    keypad(settings, TRUE);

    windows.push_back(songList);
    windows.push_back(controlBar);
    windows.push_back(settings);
    
    currWin = windows.begin(); //iterator at start of vector    
}

//initializing MPV and Fluidsynth players
void NompTUI::initPlayer()
{
    getSongs = std::make_unique<GetSongs>();
    mpvPlayer = std::make_unique<MPVPlayer>();
    fluidSynthPlayer = std::make_unique<FluidSynthPlayer>();
    files = getSongs->getSongFilePaths();
    currSong = files.begin();
}


//TODO: add pointer to files vecotr and integrate
// this with songListSelect to choose what song to play.
// Change to take currSong as reference, then update in songListSelect
void NompTUI::displaySongs()
{
    // for each song in files
    for(int fi = 0; fi<files.size(); fi++){
        // print just the name on each descending
        // converting from path > string > const char*
        filenamestr = files[fi].filename().string();
        filenameptr = filenamestr.c_str();
        if(*currSong==files[fi]){
            wattron(songList, COLOR_PAIR(NEUTRAL));
            mvwprintw(songList,2*fi+5,2,filenameptr);
            wattroff(songList, COLOR_PAIR(NEUTRAL));
        }
        else mvwprintw(songList,2*fi+5,2,filenameptr);
        wrefresh(*currWin);
    }
        
}

void NompTUI::deleteWindows()
{
    for (auto a : windows) { if (a) delwin(a); }
    windows.clear();
}


void NompTUI::displayScreen()
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
    mvwprintw(songList,2,10,"Song Queue");
    mvwprintw(currPlay,2,(getmaxx(currPlay)/2)-9,"Currently Playing");
    mvwprintw(controlBar,2,10,"Window 3");
    mvwprintw(settings,2,10,"Window 4");

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
    wrefresh(*currWin);
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
            case '\n':
            case KEY_ENTER:
                mpvPlayer->play(*currSong); //path to file (wav, flac, mp3, etc)
                //fluidSynthPlayer->play("", ""); //path to file (Midi), path to soundfont
                
                break;
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
                mpvPlayer->togglePause();
                fluidSynthPlayer->togglePause();
            default:
                wrefresh(*currWin);
                break;
        }
        break;
    }
    wbkgd(*currWin,COLOR_PAIR(0));
}

//what happens if settings is selected
void NompTUI::settingsSelect()
{
    wbkgd(*currWin,COLOR_PAIR(3));
    while(userInput!=127 && userInput!=KEY_BACKSPACE && userInput!='\b')
    {
        switch (getUserInput(*currWin))
        {
            case KEY_DOWN:
                //move down in song list
                // highlight current row
                continue;
            case KEY_UP:
                //move up in song list
                // highlight current row
                continue;
            case '\n':
            case KEY_ENTER:
                break;
            default:
                // wrefresh(*currWin);
                break;
        }
        break;
    }
    wbkgd(*currWin,COLOR_PAIR(0));
}

int NompTUI::getUserInput(WINDOW *win)
{
    userInput = wgetch(win);
    return userInput;
}

//window selection logic
// TODO: Change to use directed graph to handle smooth use input
// can do this by defining a set of movements for each window and changing functions by the current window
void NompTUI::selectWindow()
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
        else if(*currWin==settings) settingsSelect();
        else {}
        break;
        
    case 'p':
        mpvPlayer->togglePause();
        fluidSynthPlayer->togglePause();
        break;
    default:
        break;
    }
}
