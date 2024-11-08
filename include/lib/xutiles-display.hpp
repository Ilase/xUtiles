#ifndef _XUTILES_DISPLAY_HPP
#define _XUTILES_DISPLAY_HPP
#include "xutiles-include.hpp"

namespace xdr {

    class xDisplay
     {
     private:
     public:
         Display* display;
         Window root;
         int screenCount;
         Screen* defaultScreen;
         int selectedScreenId;
         Screen* selectedScreen;
         XRRScreenConfiguration* screenConfig;
         XRRScreenResources* screenResources;
         XRRScreenSize selectedScreenSize;
         int selectedScreenSizeId;
         int previousScreenSizeId;
         std::vector<std::vector<XRRScreenSize>> screenSizes;
         std::vector<short> screenRates;
         short previousRate;
         std::string screenName;
         xDisplay();
         ~xDisplay();
         void ChangeResolution(int);
         void ChangeCurrentResolutionRates(int,short,Rotation);
         void SyncChanges();
         XRRScreenSize getCurrentResolution(Screen*);
         void getSelectedRates();
         void changeScreen(int);
         void changeMonitorPositions(int, int);
         int addResolution(int, int, double);
     };

    std::string GetGraphicDeviceName();


}



#endif
