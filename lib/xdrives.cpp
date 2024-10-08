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
vector<XRRScreenSize> sizes_l = {};

bool compare_pair(pair<int, int> first, pair<int, int> second) {
    if (first.second == second.second) {
        return first.first > second.first;
    }else {
        return first.second > second.second;
    }
}

static int screenCount;
static Window root;
static Display* display;

int main(int argc, char const *argv[])
{
    display = XOpenDisplay(":0");
    Screen* screen = XScreenOfDisplay(display, DefaultScreen(display));
    root = RootWindow(display, DefaultScreen(display)); 
    screenCount = ScreenCount(display);
    std::cout  << "Detected " << screenCount << " screens" << '\n';
    auto config = XRRGetScreenInfo(display,root);
    int sizes = 0;
    auto res = XRRGetScreenResources(display, root);
    int monitorCnt;
    auto monitors = XRRGetMonitors(display, root, true, &monitorCnt);
    double dpi = (25.4 * DisplayHeight(display, 0)) / DisplayHeightMM(display, 0);
    XRRScreenSize* xrsizes = XRRSizes(display, 0, &sizes);
    for (size_t i = 0; i < sizes; i++)
    {
        sizes_l.push_back(xrsizes[i]);i
    }
    //std::sort(sizes_l.begin(), sizes_l.end(), compare_pair);
    for (size_t i = 0; i < sizes_l.size(); i++)
    {
        std::cout << i << '\t' << sizes_l[i].width << 'x' << sizes_l[i].height << '\t' << sizes_l[i].mwidth << 'x' << sizes_l[i].mheight << '\n';
        
    }
    auto size = sizes_l[5];
    std::cout << size.width << 'x' << size.height << '\n'; 
    
    XRRSetScreenConfig(display, config, root, 5, 

    //XRRSetScreenSize(display, root, size.width, size.height, size.mwidth, size.mheight);
    //XRRSetOutputPrimary(display, root, monitors[0].outputs[0]); 
    XSync(display, 0);
    //XRRSetScreenSize(display, NULL, 1000, 1000,0, 0);
    //std::cout << exec("xrandr");
    XCloseDisplay(display);
    return 0;
}
