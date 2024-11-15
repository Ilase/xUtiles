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

void xdr::xDisplay::getResolutions() {
    this->screenSizes.clear();
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
int xdr::xDisplay::addResolution(int width, int height, int hz) {
#if 0 //Запись на прямую в xorg.conf.d не работает из за недостатка прав (TODO поднятие прав для записи)
    std::fstream file("/etc/X11/xorg.conf.d/10-monitor.conf", std::ios_base::in | std::ios_base::out | std::ios_base::app);
    if(file.is_open()) {
        std::string cvt = "cvt " + std::to_string(width) + " " + std::to_string(height) + " " + std::to_string(hz);
        std::string cvtoutput = xdr::exec(cvt.c_str());
        std::string sub = cvtoutput.substr(cvtoutput.find("Modeline"), cvtoutput.rfind("\""));
        std::cout << sub << '\n';
        std::string filecontent;
        std::getline(file, filecontent, '\0');
        if (filecontent.size() < 1 || !filecontent.find(this->screenName)) {
            file << "Section " << '\"' << "Monitor" << '\"' << '\n'
             << "\tIdentifier \"" << this->screenName << "\"\n" <<
                "EndSection";
            std::getline(file, filecontent, '\0');
            std::cout << filecontent << '\n';
        }
        if (filecontent.find(sub) != std::string::npos) {
            return 1;
        }

        file.seekp(filecontent.find("EndSection",filecontent.find(this->screenName)) - 2);
        file << cvtoutput;
        return 0;
    }else {

        return 1;
    }
#endif

#if 1
    std::string cvt = "cvt " + std::to_string(width) + " " + std::to_string(height) + " " + std::to_string(hz);
    std::string cvtoutput = xdr::exec((cvt + " | sed 's/Modeline/xrandr --newmode/'").c_str());
    std::string sub = cvtoutput.substr(0, cvtoutput.find("\" ") + 1);
    sub = sub.substr(cvtoutput.find("\"") );
    std::string xpath = getenv("HOME") + std::string("/.xsessionrc");
    std::ifstream ifile(xpath);
    std::ofstream ofile(xpath, std::ios_base::app);
    if (!ifile.is_open()){
        system("chmod +x ~/.xsessionrc");
        ofile << "#!/bin/bash\n";
    }

    std::string filecontent;

    std::getline(ifile, filecontent, '\0');
    if (filecontent.find(sub) != std::string::npos){
        std::cout << "Resolution already exist\n";
        return 1;
    }
    //std::string xprofile = xdr::exec("cat ~/.xprofile");

    cvtoutput += "xrandr --addmode " + this->screenName + " " + sub;
    //std::string cvtoutput = xdr::exec(sarg.c_str()) + "xrandr --addmode " + this->screenName + " " + sub;
    /*if (file.fail()){
        std::cerr << "File failed\n";
        return 1;
    }*/
    if(ofile.fail()){
        std::cerr << std::strerror(errno) << '\n';
        return 1;
    }
    ofile << cvtoutput << '\n';
    ofile.close();
    ifile.close();
    std::string line;
    auto commands = std::istringstream(cvtoutput);
    while (getline(commands, line)) {
        if (line.find("#") == std::string::npos) {
            system(line.c_str());
        }
    }
    return 0;
#endif
}

void xdr::xDisplay::SyncChanges()
{
    XSync(display, False);
}
std::string xdr::GetGraphicDeviceName() {
    return xdr::exec("lspci | grep -E 'VGA|3D' | cut -d':' -f 3");
}

