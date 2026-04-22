//IMPORTANT
//X and Y are "flipped" in terminal
//X is up and down Y is left-right
//#include "StartPlayer.hpp"
#include "nctui.hpp"
#include <ncurses.h>
#include <vector>
#include <csignal>

volatile sig_atomic_t isResizeNeeded = false;

void handleSigwinch(int signal)
{
    isResizeNeeded = true;
}

void NompTUI::initCurses()
{
    // Check for SIGWINCH
    signal(SIGWINCH, handleSigwinch);
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

void NompTUI::deleteWindows()
{
    for (auto a : windows) { if (a) delwin(a); }
    windows.clear();
}

void NompTUI::initPlayer()
{
    mpvPlayer = std::make_unique<MPVPlayer>();
    fluidSynthPlayer = std::make_unique<FluidSynthPlayer>();
}

void NompTUI::displayScreen()
{
    //function call to read songs off of folder/playlist here
    //display text using mvwprintw([window], x, y
    const std::string title = mpvPlayer ? mpvPlayer->getMetadata("title") : "";
    const std::string artist = mpvPlayer ? mpvPlayer->getMetadata("artist") : "";
    const std::string album = mpvPlayer ? mpvPlayer->getMetadata("album") : "";

    mvwprintw(songList,2,10,"Song Queue");
    mvwprintw(currPlay,2,10,"Currently Playing");
    mvwprintw(currPlay,4,2,"Title:  %s", title.empty() ? "N/A" : title.c_str());
    mvwprintw(currPlay,5,2,"Artist: %s", artist.empty() ? "N/A" : artist.c_str());
    mvwprintw(currPlay,6,2,"Album:  %s", album.empty() ? "N/A" : album.c_str());
    mvwprintw(window3,2,10,"Window 3");
    mvwprintw(window4,2,10,"Window 4");

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
                    mpvPlayer->play("/Users/baydon/Downloads/Elijah_K - A Brighter Tomorrow.mp3");

                    //mpvPlayer->play("output.flac");
                    //fluidSynthPlayer->play("/home/briskycola/Downloads/audio/Daft Punk - Digital Love.mid", "/usr/share/soundfonts/FluidR3_GM.sf2");

                    // Give mpv a brief moment to populate metadata, then redraw once.
                    for (int i = 0; i < 20; ++i)
                    {
                        const std::string title = mpvPlayer->getMetadata("title");
                        const std::string artist = mpvPlayer->getMetadata("artist");
                        const std::string album = mpvPlayer->getMetadata("album");

                        if (!title.empty() || !artist.empty() || !album.empty())
                        {
                            break;
                        }
                        napms(50);
                    }
                    displayScreen();
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
    if (isResizeNeeded)
    {
        endwin();
        refresh();
        deleteWindows();
        initCurses();
        isResizeNeeded = false;
    }
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
        mpvPlayer->togglePause();
        //fluidSynthPlayer->togglePause();
        break;
        
    default:
        break;
    }
}
