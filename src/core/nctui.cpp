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
#include "MPVPlayer.hpp"

//Global Vars
//Add more windows here for each tab/button/thing

bool tesrunning = false;
WINDOW *songList;
WINDOW *currPlay;
WINDOW *window3;
WINDOW *window4;

std::vector<WINDOW*> windows; //vector of windows for selecting current window
std::vector<WINDOW*>::iterator currWin; //iterator for selecting current window from vector
int userInput; //this being an int and not a char is weird I know

//Ncurses initializationi


void testThread()
{
    startPlayer(2, ""); //hardcode path to song
}



void init_curses()
{
    //setlocale(LC_ALL, "");
    initscr(); //initializes ncurses
    start_color(); //starts color
    init_pair(1, COLOR_YELLOW, COLOR_MAGENTA); // color pair definition, change later, testing
    init_pair(2,COLOR_CYAN, COLOR_BLACK);
    init_pair(3,COLOR_YELLOW, COLOR_CYAN);
    noecho(); //dont show user input
    cbreak(); //all input types
    curs_set(0); //gets rid of cursor

    //initialize windows here
    songList = newwin(LINES-1, (COLS/4)-1, 1, 1); //newwin(xlength^v, ylength<>, xpos^v, ypos<>);
    currPlay = newwin(4*(LINES/6),COLS/2, 1, (COLS/4));
    window3 = newwin((LINES/3),COLS/2, 2*(LINES/3), (COLS/4));
    window4 = newwin(LINES-1,COLS/4, 1, 3*(COLS/4));
    
    //Not sure if this VVV  is necessary, i'll test lateri
    keypad(stdscr, TRUE); //allows keypad
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
    mvwprintw(songList,2,10,"Song Queue");
    mvwprintw(currPlay,2,10,"Currently Playing");
    mvwprintw(window3,2,10,"Window 3");
    mvwprintw(window4,2,10,"Window 4");

        for(auto a : windows){
        if(a==*currWin){
            wattron(a,COLOR_PAIR(1));
            box(a,0,0);
            wattroff(a,COLOR_PAIR(1));
        }else{
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
    switch(getUserInp(*currWin))
    {
        case 127:
        case KEY_BACKSPACE:
        case '\b':
            break;
        default:
            switch (userInput)
            {
                case KEY_DOWN:
                    //move down in song list
                    // highlight current row
                    break;
                case KEY_UP:
                    //move up in song list
                    // highlight current row
                    break;
                case 'o':
                    if(!tesrunning)
                    {
                    tesrunning=true;
                    std::thread tes(testThread);
                    tes.detach();
                    }
                    break;
                default:
                    wrefresh(*currWin);
                    break;
            }
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
    switch (userInput)
    {
    case KEY_LEFT:
        if(currWin!=windows.begin()){
            currWin--;
        };
        break;
    case KEY_RIGHT:
        if(currWin!=windows.end()-1){ //adding a -1 fixed      not sure why 
        currWin++;
        };
        break;
    case '\n': //enter pressed
        songListSelect(*currWin);
        break;
    case 'p':
        std::cout<<"This would've paused it normally"<<std::endl;
        //MPVPlayer::togglePause();
        break;
    default:
        break;
    }
}
