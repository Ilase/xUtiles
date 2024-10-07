#include "xdrivers.hpp"

void xdr::xdr_handler(const std::exception &e, const std::string &add_info)
{
    std::cerr << "Error: " << e.what() << std::endl;
    if(!add_info.empty()){
        std::cerr << "Additional info: " << add_info << std::endl;
    }
}

std::pair<int, int> xdr::getResolution() {
    Display* display = XOpenDisplay(NULL);
    return std::pair<int, int>(XDisplayWidth(display,0), XDisplayHeight(display,0));
}

void xdr::backup(){

    fs::path conf_path_x11 = "/etc/x11/";
    fs::path conf_path_modprobe_dir = "/etc/modprobe.d";
    //
    std::cout << "Backuping default path config" << std::endl;
    std::cout << "Current config path for backup:\n" 
        << "x11 : " << conf_path_x11 << "\n"  
        << "modeprobe_dir : " << conf_path_modprobe_dir << std::endl; 
    //
}