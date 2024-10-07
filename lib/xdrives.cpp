#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include <string>
#include <fstream>
#include <memory>
#include <cstdio>
#include <vector>
#include <utility>
#include <algorithm>

std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get()))
    {
        if (fgets(buffer, 128, pipe.get()) != NULL){
            result += buffer;
        }
    }
    return result;
}

// int FindBestVideoMode(int screen, unsigned int &width, unsigned int &height)
// {
//     int modeCount;
//     XF86VidModeModeInfo** modes;

//     if (XF86VidModeGetAllModeLines(display, screen, &modeCount, &modes))
//     {
//         for(int i = 0; i < modeCount; i ++)
//         {
//             std::cout << modes[i]->vdisplay << 'x' <<  modes[i]->hdisplay << '\n';
//         }
//         XFree(modes);
//     }
// }

// void SwitchVideoMode(int screen, int mode)
// {
//     if (mode >= 0)
//     {
//         int modeCount;
//         XF86VidModeModeInfo** modes;

//         if (XF86VidModeGetAllModeLines(display, screen, &modeCount, &modes))
//         {
//             if (mode < modeCount)
//             {
//                 XF86VidModeSwitchToMode(display, screen, modes[mode]);
//                 XF86VidModeSetViewPort(display, screen, 0, 0);


//                 XFlush(display);
//             }

//             XFree(modes);
//         }
//     }
// }
using std::vector;
using std::pair;
vector<pair<int,int>> sizes_l = {};

bool compare_pair(pair<int, int> first, pair<int, int> second) {
    if (first.second == second.second) {
        return first.first > second.first;
    }else {
        return first.second > second.second;
    }
}

int main(int argc, char const *argv[])
{
    Display* display = XOpenDisplay(":0");
    Screen* screen = XScreenOfDisplay(display, 0);
    int sizes = 0;
    XRRScreenSize* xrsizes = XRRSizes(display, 0, &sizes);
    for (size_t i = 0; i < sizes; i++)
    {
        pair size = pair(xrsizes[i].width, xrsizes[i].height);
        sizes_l.push_back(size);
    }
    std::sort(sizes_l.begin(), sizes_l.end(), compare_pair);
    for (size_t i = 0; i < sizes_l.size(); i++)
    {
        std::cout << i << '\t' << sizes_l[i].first << 'x' << sizes_l[i].second << '\n';
        
    }
    
    auto root = RootWindow(display, 0);
    XRRSetScreenSize(display, root, 1600, 900, DisplayWidthMM(display, 0),DisplayHeightMM(display, 0));
    XSync(display, 0);
    //XRRSetScreenSize(display, NULL, 1000, 1000,0, 0);
    //std::cout << exec("xrandr");
    return 0;
}
