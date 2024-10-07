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
//
bool xdr::check_existing(const fs::path &p, fs::file_status s)
{
    if(fs::status_known(s) ? fs::exists(s) : fs::exists(p)){
        return true;
    }
    return false;
}

int xdr::make_backup(fs::path &ep){

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

xdr::xDriver::xDriver(fs::path def_p){
    if(!(def_p).empty()){
        std::cout << "Default path * " << backup_path << " * changed to : " << def_p << std::endl;  
        this->backup_path = def_p;
    }
    if(!check_existing(backup_path)){
        std::cout << "Creating backup dir" << std::endl;
        fs::create_directory(backup_path);
    }
    else{
        std::cout << "Backups dir already existing on path: " << backup_path << std::endl;
    }
    for(const auto& entry : fs::directory_iterator(this->backup_path)){
        this->backups_list.push_back(entry.path().string());
    }

}

void xdr::xDriver::parse_backup_list(){
    for(const auto& entry : fs::directory_iterator(this->backup_path)){
        this->backups_list.push_back(entry.path().string());
    }
}
