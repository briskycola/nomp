//IMPORTANT
//X and Y are "flipped" in terminal
//X is up and down Y is left-right
//#include "StartPlayer.hpp"
//
// TODO: Queue: segfaults on k, doesn't play if there's more than 4 songs loaded (???)
//       Dynamic window resizing
//       Selection: Change to use directed graph to handle smooth use input
//       Add more windows
//       Use pads for screens that need to scroll
//       Iterators become invalidated if a vector is resized.
//       First song lags now, fix that
// 
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

volatile sig_atomic_t isResizeNeeded = false; //window resizing

void handleSigwinch(int signal)
{
    isResizeNeeded = true;
}

void NompTUI::initCurses() //initialize everything for ncurses
{
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
    mpvPlayer->play("");
}

void NompTUI::nextInQueue()
{
    if(mpvPlayer->isIdle() && listorqueue) //if the mpvplayer is idle, and the last song was played from the queue
    {
        queueTop++;
        if(queueTop == queue.end()) queueTop--;
        mpvPlayer->play(*queueTop);
    }
}

void NompTUI::displaySongs() //display contents of song list to songList
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
        wrefresh(songList);
    }
        
}

void NompTUI::displayQueue() //display contents of song list to songList
{
    // for each song in files
    bool hasbeenhighlighted = false;
    for(int q = 0; q<queue.size(); q++){
        // print just the name on each descending
        // converting from path > string > const char*
        queuefstr = queue[q].filename().string();
        queuefptr = queuefstr.c_str();
        if(*queueTop==queue[q] && hasbeenhighlighted==false){
            hasbeenhighlighted=true;
            wattron(queueList, COLOR_PAIR(HOVERING));
            mvwprintw(queueList,2*q+5,2,queuefptr);
            wattroff(queueList, COLOR_PAIR(HOVERING));
        }
        else mvwprintw(queueList,2*q+5,2,queuefptr);
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
    mvwprintw(songList,2,10,"Song List");
    mvwprintw(currPlay,2,(getmaxx(currPlay)/2)-9,"Currently Playing");
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
                mpvPlayer->play(*currSong); //path to file (wav, flac, mp3, etc)
                //fluidSynthPlayer->play(*currSong, ""); //path to file (Midi), path to soundfont
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
                mpvPlayer->play(*queueTop);
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
                mpvPlayer->togglePause();
                fluidSynthPlayer->togglePause();
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
                mpvPlayer->play(*queueTop);
                listorqueue = true;
                //fluidSynthPlayer->play(*queueTop, "");
                break;
            case 'c':
                queue.clear();
                queueTop=queue.begin();
                wclear(queueList);
                mpvPlayer->play("");
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
        mpvPlayer->togglePause();
        fluidSynthPlayer->togglePause();
        break;
    case 'c':
        queue.clear();
        queueTop=queue.begin();
        wclear(queueList);
        mpvPlayer->play("");
    default:
        break;
    }
}
