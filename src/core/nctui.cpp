//IMPORTANT
//X and Y are "flipped" in terminal
//X is up and down Y is left-right
#include "StartPlayer.hpp"
#include "nctui.hpp"
#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <vector>

//Global Vars
//Add more windows here for each tab/button/thing
WINDOW *songList;
WINDOW *currPlay;
WINDOW *window3;
WINDOW *window4;

std::vector<WINDOW*> windows; //vector of windows for selecting current window
std::vector<WINDOW*>::iterator currWin; //iterator for selecting current window from vector
int userInput; //this being an int and not a char is weird I know

//Ncurses initialization
void init_curses()
{
    initscr(); //initializes ncurses
    start_color(); //starts color
    init_pair(1, COLOR_BLUE, COLOR_RED); // color pair definition, change later, testing
    init_pair(2,COLOR_GREEN, COLOR_BLACK);
    init_pair(3,COLOR_WHITE, COLOR_WHITE);
    noecho(); //dont show user input
    curs_set(0); //gets rid of cursor

    //initialize windows here
    //positions have to be hard-coded
    songList = newwin(30, 25, 1, 1); //newwin(xlength, ylength, xpos, ypos);
    currPlay = newwin(25,50, 1, 30);
    window3 = newwin(25,50, 1, 90);
    window4 = newwin(15,20,1,150);
    
    //Not sure if this VVV  is necessary, i'll test later
    keypad(songList, TRUE); //allows you to use keypad
    keypad(currPlay, TRUE); //allows you to use keypad
    keypad(window3, TRUE); //allows you to use keypad
    keypad(window4, TRUE); //allows you to use keypad

    //I know this is ugly, it's the only way it works, trust

    windows.push_back(songList);
    windows.push_back(currPlay);
    windows.push_back(window3);
    windows.push_back(window4);
    
    currWin = windows.begin(); //iterator at start of vector
}

void displayScreen()
{
    //function call to read songs off of folder/playlist here
    //display text using mvwprintw([window], x, y
        for(auto a : windows){
        if(a==*currWin){
            wattron(a,COLOR_PAIR(1));
            box(a,0,0);
            wattroff(a,COLOR_PAIR(1));
			}else
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

void songListSelect(WINDOW *win)
{
    wbkgd(win,COLOR_PAIR(3));
    //another array/list for scrolling through songs? esc to exit?
    // some operation ...
    
    if(getUserInp(*currWin)==KEY_RIGHT){
     startPlayer(""); //Hardcode song path for testing   
    }
    wbkgd(win,COLOR_PAIR(0));
}

int getUserInp(WINDOW *win)
{
    userInput =  wgetch(win);
    return userInput;
}

void selectWindow()
{
    userInput = getUserInp(*currWin);
    switch (userInput){
    case KEY_LEFT:
        if(currWin!=windows.begin()){
            currWin--;
        };
        break;
    case KEY_RIGHT:
        if(currWin!=windows.end()-1){ //adding a -1 fixed      not sure why 
        currWin++; //seg fault here, not sure why    <--this
        };
        break;
    case KEY_DOWN:
        songListSelect(*currWin);
        break;
    default:
        break;
    }
}
