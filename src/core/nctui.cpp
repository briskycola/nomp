//IMPORTANT
//X and Y are "flipped" in terminal
//X is up and down Y is left-right
//#include "StartPlayer.hpp"
#include "nctui.hpp"
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <csignal>

#define HOVERING 1
#define NEUTRAL 2
#define SELECTED 3

//Initializing ncurses library tools
void NompTUI::initCurses()
{
    //setlocale(LC_ALL, "");
    initscr(); //initializes ncurses
    start_color(); //starts color
    init_pair(HOVERING, COLOR_YELLOW, COLOR_MAGENTA); // color pair definition, change later, testing
    init_pair(NEUTRAL, COLOR_CYAN, COLOR_BLACK);
    init_pair(SELECTED, COLOR_MAGENTA, COLOR_WHITE);
    noecho(); //dont show user input
    cbreak(); //all input types
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
    //windows.push_back(currPlay);
    windows.push_back(controlBar);
    windows.push_back(settings);
    
    currWin = windows.begin(); //iterator at start of vector
}




//initializing MPV and Fluidsynth players
void NompTUI::initPlayer()
{
    mpvPlayer = std::make_unique<MPVPlayer>();
    fluidSynthPlayer = std::make_unique<FluidSynthPlayer>();
}

//Displays screen
void NompTUI::displayScreen()
{
    mvwprintw(songList,2,2,"Song Queue");
    mvwprintw(currPlay,2,2,"Currently Playing");
    mvwprintw(controlBar,2,2,"Control Bar");
    mvwprintw(settings,2,2,"Settings");

    // TODO: need a display function for each window since they all
    // do different things

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

    // displaySongList();
    // ...
    
}

// Anything ending in "Select" should be interpreted as "Selected" and is what happens when each window is selected after pressing Enter

// What happens when songList is selected
void NompTUI::songListSelect()
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
                mpvPlayer->play(""); //path to file (wav, flac, mp3, etc)
                fluidSynthPlayer->play("", ""); //path to file (Midi), path to soundfont
                break;
            default:
                wrefresh(*currWin);
                break;
        }
        break;
    }
    wbkgd(*currWin,COLOR_PAIR(0));
}


// what happens when currPlay is selected
// void NompTUI::currPlaySelect()
// {
//     wbkgd(*currWin,COLOR_PAIR(3));
//     while(userInput!=127 && userInput!=KEY_BACKSPACE && userInput!='\b')
//     {
//         switch (getUserInput(*currWin))
//         {
//             case 'p':
//                 mpvPlayer->togglePause();
//                 fluidSynthPlayer->togglePause();
//                 break;
//             default:
//                 wrefresh(*currWin);
//                 break;
//         }
//         break;
//     }
//     wbkgd(*currWin,COLOR_PAIR(0));
// }

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
                wrefresh(*currWin);
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
