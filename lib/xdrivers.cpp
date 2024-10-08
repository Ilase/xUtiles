#include "xdrivers.hpp"

void xdr::xdr_handler(const std::exception &e, const std::string &add_info)
{
    std::cerr << "Error: " << e.what() << std::endl;
    if(!add_info.empty()){
        std::cerr << "Additional info: " << add_info << std::endl;
    }
}
xdr::xDisplay::xDisplay()
{
    this->display = XOpenDisplay(":0");
    this->screenCount = ScreenCount(display);
    this->defaultScreen = XDefaultScreenOfDisplay(display);
    this->root = XDefaultRootWindow(display);
    this->screenConfig = XRRGetScreenInfo(display,root);
    this->screenResources = XRRGetScreenResources(display, root);

    for (size_t i = 0; i < screenCount; i++)
    {
        int sizes;
        std::vector<XRRScreenSize> sizes_l = {};
        XRRScreenSize* xrsizes = XRRSizes(display, i, &sizes);
        for (size_t t = 0; t < sizes; t++)
        {
            sizes_l.push_back(xrsizes[t]);
        }
        this->screenSizes.push_back(sizes_l);
    }
    

}

xdr::xDisplay::~xDisplay()
{
    XCloseDisplay(display);
}

void xdr::xDisplay::ChangeResolution(XRRScreenSize *screenSize)
{
    XRRSetScreenSize(display, root, screenSize->width, screenSize->height, screenSize->mwidth, screenSize->mheight);
    SyncChanges();
}
void xdr::xDisplay::SyncChanges()
{
    XSync(display, False);
}



void xdr::ChangeResolution(int width, int height, std::string &name) {
    char buf[128];
    sprintf(buf, "xrandr --output %s --mode %dx%d", name.c_str(), width, height);
    exec(buf);
}

std::string xdr::GetGraphicDeviceName() {
    return exec("lspci | grep -E 'VGA|3D' | cut -d':' -f 3");
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

int xdr::make_backup(fs::path &ep, fs::path &x11_path, fs::path &mod_path){
    fs::path dp = ep; 
    std::ostringstream oss;
    auto timing_curr = std::chrono::system_clock::now();
    std::time_t backup_timing = std::chrono::system_clock::to_time_t(timing_curr);
    std::tm* timing_locale = std::localtime(&backup_timing);
    oss << std::put_time(timing_locale, "%Y-%m-%d--%H-%M-%S");
    //
    //
    std::string backup_name = "xdr-backup-" + oss.str();
    //
    fs::create_directory(dp / backup_name);
    dp = ep / backup_name;
    fs::create_directory(dp / "X11/");
    fs::create_directory(dp / "modprobe.d/");
    //
    //
    try{
        fs::copy(x11_path, dp / "X11/" );//, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        fs::copy(mod_path, dp / "modprobe.d/" );//, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    }catch(fs::filesystem_error& e){
        std::cerr << e.what() << std::endl;
    }catch(const std::exception& e){
        xdr::xdr_handler(e, "Error in copying files cycle");
    }
    return XDR_OK;
}

int xdr::repair_backup(fs::path &bp)
{
    
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
        this->backup_list.push_back(entry.path().string());
    }

}

xdr::xDriver::~xDriver(){
    this->backup_list.clear();
    delete& backup_path;
}

void xdr::xDriver::parse_backup_list(){
    for(const auto& entry : fs::directory_iterator(this->backup_path)){
        this->backup_list.push_back(entry.path());
    }
}



fs::path xdr::xDriver::get_backup_path()
{
    return this->backup_path;
}

fs::path xdr::xDriver::get_X11_d()
{
    return this->X11_d;
}

fs::path xdr::xDriver::get_MDP_d()
{
    return this->MDP_d; 
}
