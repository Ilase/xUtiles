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
    this->root = XDefaultRootWindow(display);


    this->screenConfig = XRRGetScreenInfo(display,root);
    this->screenResources = XRRGetScreenResources(display, root);

    XRRMonitorInfo* monitors = XRRGetMonitors(display, root, true, &this->screenCount);
    this->monitors.insert(this->monitors.end(), monitors, monitors + this->screenCount);
    for (int i = 0; i < this->screenCount; ++i) {
        XRRMonitorInfo monitor = monitors[i];
        if (monitor.primary){
            selectedScreenId = i;
            selectedScreenRROutput = monitor.outputs[0];
            selectedScreenSize = XRRScreenSize {monitor.width, monitor.height, monitor.mwidth, monitor.height};
            selectedScreenInfo = XRRGetOutputInfo(display, screenResources, selectedScreenRROutput);
            selectedScreenCrtc = XRRGetCrtcInfo(display, screenResources, selectedScreenInfo->crtc);
        }
        XRROutputInfo* output = XRRGetOutputInfo(display, screenResources, monitor.outputs[0]);
        XRRCrtcInfo* crtc = XRRGetCrtcInfo(display, screenResources, output->crtc);
        std::unordered_map<std::string, std::vector<std::pair<RRMode, float>>> modes_l = {};
        for (int j = 0; j < output->nmode; ++j) {

            for (int k = 0; k < screenResources->nmode; ++k) {
                XRRModeInfo mode = screenResources->modes[k];
                if (mode.id == output->modes[j]){
                    std::string resolution = std::to_string(mode.width) + "x" + std::to_string(mode.height);
                    double refresh = (double)mode.dotClock / ((double) mode.vTotal * (double) mode.hTotal);
                    if (modes_l.find(resolution) == modes_l.end()){
                        modes_l.insert({resolution, {std::pair<RRMode, float>(mode.id, refresh)}});
                    }else {
                        modes_l[resolution].push_back(std::pair<RRMode, float>(mode.id, refresh));
}
                }

            }

        }
        this->screenModes.push_back(modes_l);
    }
    /*for (size_t i = 0; i < screenCount; i++)
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
    }*/
}

void xdr::xDisplay::getResolutions() {

    this->screenModes.clear();
    XRRMonitorInfo* monitors = XRRGetMonitors(display, root, true, &this->screenCount);
    for (int i = 0; i < this->screenCount; ++i) {
        XRRMonitorInfo monitor = monitors[i];
        XRROutputInfo* output = XRRGetOutputInfo(display, screenResources, monitor.outputs[0]);
        std::cout << i << ' ' << output->name << '\n';
        XRRCrtcInfo* crtc = XRRGetCrtcInfo(display, screenResources, output->crtc);
        std::unordered_map<std::string, std::vector<std::pair<RRMode, float>>> modes_l = {};
        for (int j = 0; j < output->nmode; ++j) {

            for (int k = 0; k < screenResources->nmode; ++k) {
                XRRModeInfo mode = screenResources->modes[k];
                if (mode.id == output->modes[j]){
                    std::string resolution = std::to_string(mode.width) + "x" + std::to_string(mode.height);
                    double refresh = (double)mode.dotClock / ((double) mode.vTotal * (double) mode.hTotal);
                    std::cout << '\t' << resolution << ' ' << refresh << ' ' << mode.id << '\n';
                    if (modes_l.find(resolution) == modes_l.end()){
                        modes_l.insert({resolution, {std::pair<RRMode, float>(mode.id, refresh)}});
                    }else {
                        modes_l[resolution].push_back(std::pair<RRMode, float>(mode.id, refresh));
}
                }

            }

        }
        this->screenModes.push_back(modes_l);
    }

}

xdr::xDisplay::~xDisplay()
{
    XRRFreeScreenResources(screenResources);
    XRRFreeScreenConfigInfo(screenConfig);
    XCloseDisplay(display);
}


XRRScreenSize xdr::xDisplay::getCurrentResolution(){
    return XRRScreenSize {(int)this->selectedScreenCrtc->width, (int)this->selectedScreenCrtc->height, (int)this->selectedScreenInfo->mm_width ,(int)this->selectedScreenInfo->mm_height};


}

void xdr::xDisplay::ChangeResolution(int i)
{
    XRRSetScreenConfig(display, screenConfig, root, i, 1 | 0, CurrentTime);

}

void xdr::xDisplay::ChangeCurrentResolutionRates(RRMode mode, Rotation rotation) {
    /*XRRSetScreenConfigAndRate(
                display,
                screenConfig,
                root,
                sizeInd,
                rotation,
                rate,
                CurrentTime
                );*/
    //-----XRRSetCrtConfig-----//
    //----TODO: Подгон размера виртуального экрана под размеры мониторов----//
    /*
        -Выключить все мониторы
        -Просчитать нужный размер виртуального экрана
            -Просчитать dpi экрана
        -Выставить размер экрана
        -Включить обратно нужные мониторы

    */
        std::cout << this->selectedScreenCrtc->x << " " <<
                     this->selectedScreenCrtc->y << " " <<
                     this->selectedScreenCrtc->mode << " " <<
                     this->selectedScreenCrtc->rotation << " " <<
                     this->selectedScreenCrtc->noutput << " " << '\n';

        std::vector<std::pair<RRCrtc, XRRCrtcInfo>> crtcs;

        std::vector<XRRMonitorInfo> sorted_monitor = monitors;
        std::sort(sorted_monitor.begin(), sorted_monitor.end(),
            [](XRRMonitorInfo a, XRRMonitorInfo b){
            return a.x > b.x;
        }      );
        int width = 0, height = 0;
        double dpi = (double)DisplayHeight(display, 0) / (double)DisplayHeightMM(display, 0);
        for (int m = 0; m < this->screenCount; ++m) {
            if (m == selectedScreenId) {
                for (int i = 0; i < screenResources->nmode; ++i) {
                    if (screenResources->modes[i].id == mode) {
                        width += screenResources->modes[i].width;
                        height = std::max((int)screenResources->modes[i].height, height);
                    }
                }
            }else {
                width += monitors[m].width;
                height = std::max(monitors[m].height, height);
}
            XRROutputInfo *output = XRRGetOutputInfo(display, screenResources, monitors[m].outputs[0]);
            XRRCrtcInfo crtc = *XRRGetCrtcInfo(display, screenResources, output->crtc);
            crtcs.push_back({output->crtc, crtc});
            //Disable crtc
            XRRSetCrtcConfig(display, screenResources, output->crtc,
                             CurrentTime, 0,0, 0, RR_Rotate_0, NULL, 0);
        }
        std::cout << width << 'x' << height << '\n' << width / dpi << 'x' << height / dpi << '\n';
        XRRSetScreenSize(display, root, width, height, width / dpi, height / dpi);
        for (int m = 0; m < screenCount; ++m) {
            int s = 0;
            RRCrtc crtc_id = crtcs[m].first;
            XRRCrtcInfo crtc = crtcs[m].second;

            if(m == selectedScreenId){
                s = XRRSetCrtcConfig(display,
                                     screenResources,
                                     crtc_id,
                                     CurrentTime,
                                     crtc.x,
                                     crtc.y,
                                     mode,
                                     rotation,
                                     crtc.outputs,
                                     crtc.noutput);
            }else {

                XRRSetCrtcConfig(display,
                                 screenResources,
                                 crtc_id,
                                 CurrentTime,
                                 crtc.x,
                                 crtc.y,
                                 crtc.mode,
                                 crtc.rotation,
                                 crtc.outputs,
                                 crtc.noutput);
            }
            if (s != Success){
                std::cout << " " << this->selectedScreenInfo->crtc << "\tERROR" << s << "\n";

            }
            std::cout << s  << '\n';
        }
       /* auto s = XRRSetCrtcConfig(display,
                                  screenResources,
                                  this->selectedScreenInfo->crtc,
                                  CurrentTime,
                                  this->selectedScreenCrtc->x,
                                  this->selectedScreenCrtc->y,
                                  mode,
                                  rotation,
                                  this->selectedScreenCrtc->outputs,
                                  this->selectedScreenCrtc->noutput);
        if (s != Success){
            std::cout << " " << this->selectedScreenInfo->crtc << "\tERROR" << s << "\n";
        }*/
        SyncChanges();

}

void xdr::xDisplay::changeScreen(int screenID) {
    this->selectedScreenId = screenID;
    std::cout << screenID << ' ' << monitors.size() << '\n';
    this->selectedScreenInfo = XRRGetOutputInfo(this->display, screenResources, monitors[screenID].outputs[0]);
    std::cout << selectedScreenInfo << '\n';
    this->selectedScreenCrtc = XRRGetCrtcInfo(display, screenResources, selectedScreenInfo->crtc);
    std::cout << selectedScreenCrtc << '\n';
    selectedScreenSize = XRRScreenSize {monitors[screenID].width, monitors[screenID].height, monitors[screenID].mwidth ,monitors[screenID].mheight};
    previousMode = this->selectedScreenCrtc->mode;

    /*this->selectedScreen = XScreenOfDisplay(display, screenID);
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
    }*/
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

void xdr::xDisplay::updatePreviousMode() {
    XRROutputInfo* output = XRRGetOutputInfo(this->display, screenResources, screenResources->outputs[selectedScreenId]);
    auto crtc = XRRGetCrtcInfo(display, screenResources, output->crtc);
    selectedScreenSize = XRRScreenSize {(int)crtc->width, (int)crtc->height, (int)output->mm_width ,(int)output->mm_height};
    previousMode = crtc->mode;
}


void xdr::xDisplay::SyncChanges()
{
    XSync(display, False);
}
std::string xdr::GetGraphicDeviceName() {
    return xdr::exec("lspci | grep -E 'VGA|3D' | cut -d':' -f 3");
}

