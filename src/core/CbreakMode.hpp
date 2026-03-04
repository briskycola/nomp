// This header contains code that changes
// the terminal to cbreak mode and reads
// input from the keyboard.
//
// NOTE: This code is only used for debugging
// and testing the core. It won't be in
// the final build.

#include <termios.h>

bool enableCbreakMode(termios &original);
bool disableCbreakMode(termios &original);
char getCharFromKeyboard();
