#include <ncurses.h>
#ifndef nctui
#define nctui

void init_curses();
void displayScreen();
void songListSelect(WINDOW *win);
int getUserInp(WINDOW *win);
void selectWindow();

#endif
