#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    Display* display = XOpenDisplay(NULL);
    int screen_count = XScreenCount(display);
    int min_width, min_height, max_width, max_height;
    XRRGetScreenSizeRange(display, NULL, &min_width, &min_height, &max_width, &max_height);
    std::cout << min_width << 'x' << min_height << '\n';
    std::cout << max_width << 'x' << max_height << '\n';
    
    

    return 0;
}
