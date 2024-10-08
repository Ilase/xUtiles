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
#include <sstream>
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
    class xDriver
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
        xDriver();
        ~xDriver();
        void ChangeResolution(XRRScreenSize* screenSize);
        void SyncChanges();
    };
    
    xDriver::xDriver()
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
    
    xDriver::~xDriver()
    {
        XCloseDisplay(display);
    }
    
    void xDriver::ChangeResolution(XRRScreenSize *screenSize)
    {
        XRRSetScreenSize(display, root, screenSize->width, screenSize->height, screenSize->mwidth, screenSize->mheight);
        SyncChanges();
    }
    void xDriver::SyncChanges()
    {
        XSync(display, False);
    }

    
    
    void ChangeResolution(int width, int height, std::string &name) {
        char buf[128];
        sprintf(buf, "xrandr --output %s --mode %dx%d", name.c_str(), width, height);
        exec(buf);
    }

    std::string GetGraphicDeviceName() {
        return exec("lspci | grep -E 'VGA|3D' | cut -d':' -f 3");
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
    // auto xDisplay = xdr::XDisplay();
    // auto res = exec("xrandr -q | grep ' connected'");
    // std::istringstream line = std::istringstream(res);
    // std::string name;
    // getline(line, name, ' ');
    // std::cout << name;
    // std::cout << exec("xrandr -q");
    // std::cout << "Select resolution (for example 320x200)\n";
    // int size_x, size_y;
    // scanf("%dx%d", &size_x, &size_y);
    // char buf[128];
    // sprintf(buf, "xrandr --output %s --mode %dx%d", name.c_str(), size_x, size_y);
    // std::cout << exec(buf);
    std::cout << xdr::GetGraphicDeviceName();
    return 0;
}
