#ifndef _XUTILES_DISPLAY_HPP
#define _XUTILES_DISPLAY_HPP

#include "xutiles-include.hpp"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusArgument>
#include <QSize>
#include <unordered_map>
#include <KF5/KScreen/kscreen/config.h>
#include <KF5/KScreen/kscreen/backendmanager_p.h>

namespace xdr {

    class xDisplay
     {
     private:
        struct Mode{
            unsigned int width, height;
            std::vector<std::pair<RRMode, float>> rates;
        };
     public:
         Display* display;
         Window root;
         int screenCount;
         int selectedScreenId;
         RROutput selectedScreenRROutput;
         XRROutputInfo* selectedScreenInfo;
         XRRCrtcInfo* selectedScreenCrtc;
         XRRScreenConfiguration* screenConfig;
         XRRScreenResources* screenResources;
         XRRScreenSize selectedScreenSize;
         RRMode previousMode;
         std::vector<XRRMonitorInfo> monitors;
         std::vector<std::unordered_map<std::string, std::vector<std::pair<RRMode, float>>>> screenModes;
         std::string screenName;
         xDisplay();
         ~xDisplay();
         void ChangeResolution(int);
         void ChangeCurrentResolutionRates(RRMode,Rotation);
         void SyncChanges();
         XRRScreenSize getCurrentResolution();
         void changeScreen(int);
         void changeMonitorPositions(int, int);
         void getResolutions();
         int addResolution(int, int, int);
         void updatePreviousMode();
     };

    std::string GetGraphicDeviceName();


}



#endif
