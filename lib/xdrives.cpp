#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/xf86vmode.h>
#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include <string>
#include <fstream>
#include <memory>
#include <cstdio>

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


int main(int argc, char const *argv[])
{
    Display* display = XOpenDisplay(":0");
    Screen* screen = XScreenOfDisplay(display, 0);
    // XRRScreenResources *screen = XRRGetScreenResources(display, DefaultRootWindow(display));
    // for (size_t i = 0; i < screen->ncrtc; i++)
    // {
    //     XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(display, screen, screen->crtcs[i]);  
    //     std::cout << i << '\t' << crtc_info->width << 'x' << crtc_info-> height << '\n';
    //     /* code */
    // }
    int modeCount;
    XF86VidModeModeInfo** modes;
    int sizes;
    if (XF86VidModeGetAllModeLines(display, 0, &modeCount, &modes))
    {
        for(int i = 0; i < modeCount; i ++)
        {
            //std::cout << i << '\t' << modes[i]->hdisplay << 'x' <<  modes[i]->vdisplay << '\t' << modes[i]->vtotal << '\n';
        }
        XFree(modes);
    }
    XRRScreenSize* xrsizes = XRRSizes(display, 0, &sizes);
    for (size_t i = 0; i < sizes; i++)
    {
        //std::cout << xrsizes[i].width << 'x' << xrsizes[i].height << '\n';
    }
    auto root = RootWindow(display, 0);
    XRRSetScreenSize(display, root, 1600, 900, DisplayWidthMM(display, 0),DisplayHeightMM(display, 0));
    XSync(display, 0);
    //XRRSetScreenSize(display, NULL, 1000, 1000,0, 0);
    //std::cout << exec("xrandr");
    return 0;
}
