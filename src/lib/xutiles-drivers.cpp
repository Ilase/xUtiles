#include "xutiles-drivers.hpp"



std::pair<int, int> xdr::getResolution()
{
    Display *display = XOpenDisplay(NULL);
    return std::pair<int, int>(XDisplayWidth(display, 0), XDisplayHeight(display, 0));
}
//
//
