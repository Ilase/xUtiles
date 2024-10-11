#include "xdrivers.hpp"

void xdr::xdr_handler(const std::exception &e, const std::string &add_info)
{
    std::cerr << "Error: " << e.what() << std::endl;
    if(!add_info.empty()){
        std::cerr << "Additional info: " << add_info << std::endl;
    }
}
//
// int xdr::xDriver::make_backup()
// {   
//     xdr::make_backup(this->backup_path, );
//     return 0;
// }
//
std::pair<int, int> xdr::getResolution()
{
    Display* display = XOpenDisplay(NULL);
    return std::pair<int, int>(XDisplayWidth(display,0), XDisplayHeight(display,0));
}
//
//
bool xdr::check_existing(const fs::path &p, fs::file_status s)
{
    if(fs::status_known(s) ? fs::exists(s) : fs::exists(p)){
        return true;
    }
    return false;
}

int xdr::make_backup(fs::path &_bp, fs::path &x11_path, fs::path &mod_path){
    fs::path dp = _bp; 
    // Getting folder name
    std::ostringstream oss;
    auto timing_curr = std::chrono::system_clock::now();
    std::time_t backup_timing = std::chrono::system_clock::to_time_t(timing_curr);
    std::tm* timing_locale = std::localtime(&backup_timing);
    oss << std::put_time(timing_locale, "%Y-%m-%d--%H-%M-%S");
    std::string backup_name = "xdr-backup-" + oss.str();
    //
    //
    fs::create_directory(dp / backup_name);
    dp = _bp / backup_name;
    fs::create_directory(dp / "X11/");
    fs::create_directory(dp / "modprobe.d/");
    //
    // Try to copy files
    try{
        fs::copy(x11_path, dp / "X11/" );//, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        fs::copy(mod_path, dp / "modprobe.d/" );//, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    }catch(fs::filesystem_error& e){
        std::cerr << e.what() << std::endl;
        return XDR_ERR;
    }catch(const std::exception& e){
        return XDR_ERR;
        xdr::xdr_handler(e, "Error in copying files cycle");
    }
    return XDR_OK;
}
/// @brief 
/// @param bp Path to bacup folder  
/// @return xdr_stat OK 
int xdr::repair_backup(fs::path &bp)
{
    fs::path x11_p = "/etc/X11/", mdp_p = "/etc/modprobe.d";
    if(!xdr::check_existing(bp) && !fs::exists(x11_p) && fs::exists(mdp_p)){
        std::cerr << XDR_PREF << "Backup path not valid or target directory not existing! "<< std::endl;
        return XDR_ERR;
    }
    try{
        copy(bp / "modprobe.d", mdp_p, fs::copy_options::update_existing);
        copy(bp / "modprobe.d", mdp_p, fs::copy_options::update_existing);
    }catch(fs::filesystem_error &fse){
        std::cerr << XDR_PREF << fse.what() << std::endl;        
    }catch(std::exception &e){
        xdr::xdr_handler(e, "Error occured while repairing backup.");
    }
    
    return XDR_OK;
}

xdr::xBackup::xBackup(fs::path def_p){
    //Get username
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    this->username = pw->pw_name; 
    //
    this->backup_path = fs::path("/home/") / username / ".xrd-backups/";
    if(!(def_p).empty()){
        std::cout << XDR_PREF << "Default path * " << backup_path << " * changed to : " << def_p << std::endl;  
        this->backup_path = def_p;
    }
    if(!check_existing(backup_path)){
        std::cout << XDR_PREF << "Creating backup dir" << std::endl;
        fs::create_directory(backup_path);
    }
    else{
        //std::cout << XDR_PREF << "Backups folder loacate in path: " << backup_path << std::endl;
    }
    parse_backup_list();

}

// xdr::xDriver::~xDriver(){
//     this->backup_list.clear();
//     delete& backup_path;
// }

void xdr::xBackup::parse_backup_list() noexcept {
    this->backup_list.clear();
    for(const auto& entry : fs::directory_iterator(this->backup_path)){
        this->backup_list.push_back(entry.path());
    }
}

int xdr::xBackup::make_backup()
{
    if (xdr::make_backup(this->backup_path,this->X11_d, this->MDP_d) == XDR_ERR)
        return XDR_ERR;
    parse_backup_list();
    return XDR_OK;
}

std::vector<fs::path> xdr::xBackup::get_backup_list()
{
    return this->backup_list;
}

fs::path xdr::xBackup::get_backup_path()
{
    return this->backup_path;
}

fs::path xdr::xBackup::get_X11_d()
{
    return this->X11_d;
}

fs::path xdr::xBackup::get_MDP_d()
{
    return this->MDP_d; 
}

std::string xdr::xBackup::get_username()
{
    return this->username;
}

void xdr::xBackup::print_backup_list()
{
    for(const auto& _bpath : backup_list){
        std::cout << XDR_PREF << _bpath << std::endl;
    }
}
