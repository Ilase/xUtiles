#include <X11/Xlib.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    Display* display = XOpenDisplay(NULL);
    Screen* screen = XScreenOfDisplay(display, 0);
    std::cout << XWidthOfScreen(screen) << 'x' << XHeightOfScreen(screen) << '\n';


    return 0;
}
