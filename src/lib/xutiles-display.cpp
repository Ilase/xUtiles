#include "xutiles-display.hpp"
#include "xutiles.hpp"

xdr::xDisplay::xDisplay()
{
    auto res = xdr::exec("xrandr -q | grep ' connected'");
    std::istringstream line = std::istringstream(res);
    std::string name;
    getline(line, name, ' ');
    this->screenName = name;
    this->display = XOpenDisplay(":0");
    this->screenCount = ScreenCount(display);
    this->selectedScreen = this->defaultScreen = XDefaultScreenOfDisplay(display);
    this->selectedScreenId = XScreenNumberOfScreen(selectedScreen);
    this->root = XDefaultRootWindow(display);
    this->screenConfig = XRRGetScreenInfo(display,root);
    this->screenResources = XRRGetScreenResources(display, root);
    selectedScreenSize = getCurrentResolution(selectedScreen);
    previousRate = XRRConfigCurrentRate(screenConfig);
    for (size_t i = 0; i < screenCount; i++)
    {
        int sizes;
        std::vector<XRRScreenSize> sizes_l = {};
        XRRScreenSize* xrsizes = XRRSizes(display, i, &sizes);
        for (size_t t = 0; t < sizes; t++)
        {
            auto size = xrsizes[t];
            if((i == selectedScreenId) && (size.width == selectedScreenSize.width) && (size.height == selectedScreenSize.height)) {
                previousScreenSizeId = selectedScreenSizeId = t;
            }
            sizes_l.push_back(size);
        }
        this->screenSizes.push_back(sizes_l);
    }
    getSelectedRates();

}

xdr::xDisplay::~xDisplay()
{
    XRRFreeScreenResources(screenResources);
    XRRFreeScreenConfigInfo(screenConfig);
    XCloseDisplay(display);
}


XRRScreenSize xdr::xDisplay::getCurrentResolution(Screen *screen){
    return XRRScreenSize {
        WidthOfScreen(screen),
        HeightOfScreen(screen),
        WidthMMOfScreen(screen),
        HeightMMOfScreen(screen),
    };
}

void xdr::xDisplay::ChangeResolution(int i)
{
    XRRSetScreenConfig(display, screenConfig, root, i, 1 | 0, CurrentTime);

}

void xdr::xDisplay::ChangeCurrentResolutionRates(int sizeInd, short rate, Rotation rotation) {
    XRRSetScreenConfigAndRate(
                display,
                screenConfig,
                root,
                sizeInd,
                rotation,
                rate,
                CurrentTime
                );
}

void xdr::xDisplay::getSelectedRates() {
    screenRates.clear();
    int nrates;
    short *rates = XRRRates(display, selectedScreenId, selectedScreenSizeId, &nrates);
    for (int i = 0; i < nrates; ++i) {
        screenRates.push_back(rates[i]);
    }
}

void xdr::xDisplay::changeScreen(int screenID) {
    this->selectedScreen = XScreenOfDisplay(display, screenID);
    this->selectedScreenId = XScreenNumberOfScreen(selectedScreen);
    this->root = XDefaultRootWindow(display);
    this->screenConfig = XRRGetScreenInfo(display,root);
    this->screenResources = XRRGetScreenResources(display, root);
    selectedScreenSize = getCurrentResolution(selectedScreen);
    int sizes;
    auto xrsizes = screenSizes[selectedScreenId];
    for (size_t t = 0; t < sizes; t++)
    {
        auto size = xrsizes[t];
        if((size.width == selectedScreenSize.width) && (size.height == selectedScreenSize.height)) {
            previousScreenSizeId = selectedScreenSizeId = t;
        }
    }
}

int xdr::xDisplay::addResolution(int width, int height, double hz) {
    char buf[32];
    sprintf(buf, "%dx%d_%.2f", width, height, hz);
    std::string sbuf = buf;
    sbuf = sbuf.replace(sbuf.find(","),1, ".");
    std::cout << sbuf << '\n';
    std::string xpath = getenv("HOME") + std::string("/.xsessionrc");
    std::ifstream xprofile;
    xprofile.open(xpath);
    std::string filecontent;
    std::getline(xprofile, filecontent, '\0');
    //std::string xprofile = xdr::exec("cat ~/.xprofile");
    if (filecontent.find(sbuf) != std::string::npos){
        return 1;
    }
    char arg[128];
    sprintf(arg, "cvt %d %d %.2f | sed 's/Modeline/xrandr --newmode/'", width, height, hz);
    std::string sarg = arg;
    sarg = sarg.replace(sarg.find(","),1, ".");
    std::string cvtoutput = xdr::exec(sarg.c_str()) + "xrandr --addmode " + this->screenName + " " + sbuf;
    std::ofstream file;
    file.open(xpath, std::ios_base::app);
    if (file.fail()){
        return 1;
    }
    file << cvtoutput << '\n';
    system(xpath.c_str());
    return 0;
}

void xdr::xDisplay::SyncChanges()
{
    XSync(display, False);
}
std::string xdr::GetGraphicDeviceName() {
    return xdr::exec("lspci | grep -E 'VGA|3D' | cut -d':' -f 3");
}

