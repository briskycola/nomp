#if defined(__linux__) || defined(__APPLE__)
#include "CbreakMode.hpp"
#include <iostream>
#include <unistd.h>
#include <poll.h>

bool enableCbreakMode(termios &original)
{
    termios cbreak;

    // Save original terminal attributes.
    if (tcgetattr(STDIN_FILENO, &original) == -1)
    {
        std::cerr << "Could not save original terminal attributes\n";
        return false;
    }

    // Copy original terminal attributes
    // to cbreak.
    cbreak = original;
    
    // Now we switch to cbreak mode.

    // Disable canonical mode.
    //
    // This makes it so that when you press
    // a key, the input is processed instantly.
    // No need to press enter.
    cbreak.c_lflag &= ~ICANON;

    // Disable echo.
    //
    // Makes it so you can't see the input
    // on the screen.
    cbreak.c_lflag &= ~ECHO;

    // Now we apply the new settings.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &cbreak) == -1)
    {
        std::cerr << "Could not apply new terminal attributes\n";
        return false;
    }
    return true;
}

bool disableCbreakMode(termios &original)
{
    // Reset terminal attributes
    // back to original.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original) == -1)
    {
        std::cerr << "Could not reset to original terminal attributes\n";
        return false;
    }
    return true;
}

char getCharFromKeyboard()
{
    char ch;
    pollfd keyboard;
    keyboard.fd = STDIN_FILENO;
    keyboard.events = POLLIN;

    if (poll(&keyboard, 1, 0) > 0 && keyboard.revents & (POLLIN | POLLHUP))
    {
        if (read(STDIN_FILENO, &ch, sizeof(ch)) == 1)
        {
            return ch;
        }
    }
    return '\0';
}
#endif
