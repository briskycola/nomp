//IMPORTANT
//X and Y are "flipped" in terminal
//X is up and down Y is left-right
#include "StartPlayer.hpp"
#include "nctui.hpp"
#include <cstdint>
#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <thread>

//Global Vars
//Add more windows here for each tab/button/thing

MPVPlayer* playerptr;

void testThread()
{
    playerptr = &startPlayer(2, "/Users/rileywhite/TerminalStuff/sftwr380/nomp/songs/Rosalina_Observatory_2.mp3"); //hardcode path to song, first arg: 2 = music file, 3 = midi
}

void NompTUI::init_curses()
{
    
    //setlocale(LC_ALL, "");
    initscr(); //initializes ncurses
    start_color(); //starts color
    init_pair(1, COLOR_YELLOW, COLOR_MAGENTA); // color pair definition, change later, testing
    init_pair(2,COLOR_CYAN, COLOR_BLACK);
    init_pair(3,COLOR_MAGENTA, COLOR_WHITE);
    noecho(); //dont show user input
    cbreak(); //all input types
    curs_set(0); //gets rid of cursor

    //initialize windows here
    songList = newwin(LINES-1, (COLS/4)-1, 1, 1); //newwin(xlength (up down), ylength (left right), xpos, ypos<>);
    currPlay = newwin(4*(LINES/6),COLS/2, 1, (COLS/4));
    window3 = newwin((LINES/3),COLS/2, 2*(LINES/3), (COLS/4));
    window4 = newwin(LINES-1,COLS/4, 1, 3*(COLS/4));
    
    keypad(stdscr, TRUE); //allows keypad
    keypad(currPlay, TRUE);
    keypad(songList, TRUE);
    keypad(window3, TRUE);
    keypad(window4, TRUE);


    windows.push_back(songList);
    windows.push_back(currPlay);
    windows.push_back(window3);
    windows.push_back(window4);
    
    currWin = windows.begin(); //iterator at start of vector
}

void NompTUI::displayScreen()
{
    //function call to read songs off of folder/playlist here
    //display text using mvwprintw([window], x, y
    mvwprintw(songList,2,10,"Song Queue");
    mvwprintw(currPlay,2,10,"Currently Playing");
    mvwprintw(window3,2,10,"Window 3");
    mvwprintw(window4,2,10,"Window 4");

        for(auto a : windows)
        {
        if(a==*currWin)
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
}

//We can put a case for "ENTER" that selects the window and runs
//another method specific to each window 

void NompTUI::songListSelect(WINDOW *win)
{
    wbkgd(win,COLOR_PAIR(3));
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
            //case '\n':
            //case KEY_ENTER:
            case 'o':
                if(!tesRunning)
                {
                    tesRunning=true;
                    std::thread tes(testThread);
                    tes.detach();
                }
                break;
            default:
                wrefresh(*currWin);
                break;
        }
        break;
    }
    wbkgd(win,COLOR_PAIR(0));
}

int NompTUI::getUserInput(WINDOW *win)
{
    userInput = wgetch(win);
    return userInput;
}

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
        songListSelect(*currWin);
        break;
        
    case 'p':
        playerptr->togglePause();
        break;
        
    default:
        break;
    }
}
