#include "nctui.hpp"

void display(NompTUI &tui)
{
    while(tui.userInput != 27)
    {
        tui.displayScreen();
        tui.selectWindow();
    }
}

int main(int argc, char **argv)
{
    NompTUI tui;

    // FluidSynth has a lot of error output
    // on Linux that is just pure noise. It's really
    // just ALSA trying to find audio devices. It's
    // not important, and there's no clear way to fix
    // it, so we will just throw away all stderr.
#if defined(__linux__)
    freopen("/dev/null", "w", stderr);
#endif
    
    tui.initCurses();
    display(tui);
    endwin();
    return 0;
}
