#include "xdrivers.hpp"

void xdr::xdr_handler(const std::exception &e, const std::string &add_info)
{
    std::cerr << "Error: " << e.what() << std::endl;
    if(!add_info.empty()){
        std::cerr << "Additional info: " << add_info << std::endl;
    }
}

// int xdr::xDriver::make_backup()
// {   
//     xdr::make_backup(this->backup_path, );
//     return 0;
// }

std::pair<int, int> xdr::getResolution()
{
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

int xdr::make_backup(fs::path &ep, fs::path &x11_path, fs::path &mod_path)
{   
    try{
        fs::copy(x11_path, ep);
        fs::copy(mod_path, ep);
    }catch(fs::filesystem_error& e){
        std::cerr << e.what() << std::endl;
        return XDR_ERR;
    }
    return XDR_OK;
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
