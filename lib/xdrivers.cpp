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

bool xdr::check_existing(const fs::path &p, fs::file_status s)
{
    if(fs::status_known(s) ? fs::exists(s) : fs::exists(p)){
        return true;
    }
    return false;
}

int xdr::backup(){

    fs::path conf_path_x11 = "/etc/X11/";
    fs::path conf_path_modprobe_dir = "/etc/modprobe.d";
    fs::path conf_backup_dir = "/etc/xdrives/backups";
    //
    std::cout << "Backuping default path config" << std::endl;
    std::cout << "Current config path for backup:\n" 
        << "x11 : " << conf_path_x11 << "\n"  
        << "modeprobe : " << conf_path_modprobe_dir << std::endl; 
    //
    if(!check_existing(conf_path_x11)){
        std::cout << conf_path_x11 << " does not exists!" << std::endl;
        return XDR_ERR;
    }
    if(!check_existing(conf_path_modprobe_dir)){
        std::cout << conf_path_modprobe_dir << " does not exists!" << std::endl;
        return XDR_ERR;
    }
    
    //
    char ans;
    std::cout << "Process the backuping configuration? : ";
    std::cin >> ans;
    //
    switch(ans){
        case 'y':
        break;
        case 'n':
        break;
        default: 
            std::cerr << "Unknown char!";
            return XDR_ERR;
            break;
    } 
    return 0;
}