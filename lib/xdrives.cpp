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

namespace xdr {
    class XDisplay
    {
    private:
    public:
        Display* display;
        Window root;
        int screenCount;
        Screen* defaultScreen;
        XRRScreenConfiguration* screenConfig;
        XRRScreenResources* screenResources;
        std::vector<std::vector<XRRScreenSize>> screenSizes;
        XDisplay();
        ~XDisplay();
        void ChangeResolution(XRRScreenSize* screenSize);
        void SyncChanges();
    };
    
    XDisplay::XDisplay()
    {
        this->display = XOpenDisplay(":0");
        this->screenCount = ScreenCount(display);
        this->defaultScreen = XDefaultScreenOfDisplay(display);
        this->root = XDefaultRootWindow(display);
        this->screenConfig = XRRGetScreenInfo(display,root);
        this->screenResources = XRRGetScreenResources(display, root);

        for (size_t i = 0; i < screenCount; i++)
        {
            int sizes;
            std::vector<XRRScreenSize> sizes_l = {};
            XRRScreenSize* xrsizes = XRRSizes(display, i, &sizes);
            for (size_t t = 0; t < sizes; t++)
            {
                sizes_l.push_back(xrsizes[t]);
            }
            this->screenSizes.push_back(sizes_l);
        }
        

    }
    
    XDisplay::~XDisplay()
    {
        XCloseDisplay(display);
    }
    
    void XDisplay::ChangeResolution(XRRScreenSize *screenSize)
    {
        XRRSetScreenSize(display, root, screenSize->width, screenSize->height, screenSize->mwidth, screenSize->mheight);
        SyncChanges();
    }
    void XDisplay::SyncChanges()
    {
        XSync(display, False);
    }
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


int main(int argc, char const *argv[])
{
    auto xDisplay = xdr::XDisplay();
    std::cout  << "Detected " << xDisplay.screenCount << " screens" << '\n';
    int sizes = 0;
    int monitorCnt;
    auto monitors = XRRGetMonitors(xDisplay.display,xDisplay.root, true, &monitorCnt);
    double dpi = (25.4 * DisplayHeight(xDisplay.display, 0)) / DisplayHeightMM(xDisplay.display, 0);
    
    //std::sort(sizes_l.begin(), sizes_l.end(), compare_pair);
    for (size_t t = 0; t < xDisplay.screenSizes.size(); t++)
    {
        auto sizes_l = xDisplay.screenSizes[t];
        std::cout << "Screen " << t << " resolutions:\n";
        for (size_t i = 0; i < sizes_l.size(); i++)
        {
            std::cout << i << '\t' << sizes_l[i].width << 'x' << sizes_l[i].height << '\t' << sizes_l[i].mwidth << 'x' << sizes_l[i].mheight << '\n';
        }
    }
    
    auto size = xDisplay.screenSizes[0][5];
    std::cout << size.width << 'x' << size.height << '\n'; 
    xDisplay.ChangeResolution(&size);
    return 0;
}
