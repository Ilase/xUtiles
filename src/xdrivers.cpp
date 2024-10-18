#include "xdrivers.hpp"

void xdr::xdr_handler(const std::exception &e, const std::string &add_info)
{
    std::cerr << "Error: " << e.what() << std::endl;
    if (!add_info.empty())
    {
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
std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get()))
    {
        if (fgets(buffer, 128, pipe.get()) != NULL){
            result += buffer;
        }
    }
    return result;
}
xdr::xDisplay::xDisplay()
{
    auto res = exec("xrandr -q | grep ' connected'");
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
    for (size_t i = 0; i < screenCount; i++)
    {
        int sizes;
        std::vector<XRRScreenSize> sizes_l = {};
        XRRScreenSize* xrsizes = XRRSizes(display, i, &sizes);
        for (size_t t = 0; t < sizes; t++)
        {
            auto size = xrsizes[t];
            if((i == selectedScreenId) && (size.width == selectedScreenSize.width) && (size.height == selectedScreenSize.height)) {
                selectedScreenSizeId = t;
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
            selectedScreenSizeId = t;
        }
    }
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

xdr::xDriver::xDriver() {
    std::string output = exec("/opt/xUtils/parser");
    std::string line;
    auto stream = std::istringstream(output);
    while (getline(stream, line)) {
        this->versions.push_back(line);
    }
}

std::string xdr::xDriver::getLinkToVersion(std::string& version) {
    return std::string("https://us.download.nvidia.com/XFree86/Linux-x86_64/") + version + std::string("/NVIDIA-Linux-x86_64-") + version + std::string(".run");
}

void xdr::xDriver::downloadVersion(std::string& version) {
    std::string link = getLinkToVersion(version);
    //system("echo 1");
    system((std::string("wget ") + link).c_str());
}

std::vector<std::string>& xdr::xDriver::getVersions() {
    return this->versions;
}

std::pair<int, int> xdr::getResolution()
{
    Display* display = XOpenDisplay(NULL);
    return std::pair<int, int>(XDisplayWidth(display,0), XDisplayHeight(display,0));
}
//
//
bool xdr::check_existing(const fs::path &p, fs::file_status s)
{
    // if (fs::status_known(s) ? fs::exists(s) : fs::exists(p))
    // {
    //     return true;
    // }
    // return false;
    return fs::status_known(s) ? fs::exists(s) : fs::exists(p);
}

int xdr::make_backup(fs::path &_bp, fs::path &x11_path, fs::path &mod_path)
{
    fs::path dp = _bp;
    // Getting folder name
    std::ostringstream oss;
    auto timing_curr = std::chrono::system_clock::now();
    std::time_t backup_timing = std::chrono::system_clock::to_time_t(timing_curr);
    std::tm *timing_locale = std::localtime(&backup_timing);
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
    try
    {
        fs::copy(x11_path, dp / "X11/");        //, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        fs::copy(mod_path, dp / "modprobe.d/"); //, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    }
    catch (fs::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
        return XDR_ERR;
    }
    catch (const std::exception &e)
    {
        return XDR_ERR;
        xdr::xdr_handler(e, "Error in copying files cycle");
    }
    return XDR_OK;
}
/// @brief Repair conf files from path folder
/// @param bp Path to bacup folder
/// @return xdr_stat OK
int xdr::repair_backup(fs::path &bp)
{
    fs::path x11_p = "/etc/X11/", mdp_p = "/etc/modprobe.d";
    if (!xdr::check_existing(bp) && !fs::exists(x11_p) && fs::exists(mdp_p))
    {
        std::cerr << XDR_PREF << "Backup path not valid or target directory not existing! " << std::endl;
        return XDR_ERR;
    }
    try
    {
        copy(bp / "modprobe.d", mdp_p, fs::copy_options::update_existing);
        copy(bp / "modprobe.d", mdp_p, fs::copy_options::update_existing);
    }
    catch (fs::filesystem_error &fse)
    {
        std::cerr << XDR_PREF << fse.what() << std::endl;
    }
    catch (std::exception &e)
    {
        xdr::xdr_handler(e, "Error occured while repairing backup.");
    }

    return XDR_OK;
}

std::string xdr::get_username()
{
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    return pw->pw_name;
}

xdr::xBackup::xBackup(fs::path def_p)
{
    // Get username
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    this->username = pw->pw_name;
    //
    this->backup_path = fs::path("/home/") / username / ".xrd-backups/";
    if (!(def_p).empty())
    {
        std::cout << XDR_PREF << "Default path * " << backup_path << " * changed to : " << def_p << std::endl;
        this->backup_path = def_p;
    }
    if (!check_existing(backup_path))
    {
        std::cout << XDR_PREF << "Creating backup dir" << std::endl;
        fs::create_directory(backup_path);
    }
    else
    {
        // std::cout << XDR_PREF << "Backups folder loacate in path: " << backup_path << std::endl;
    }
    parse_backup_list();
}

// xdr::xDriver::~xDriver(){
//     this->backup_list.clear();
//     delete& backup_path;
// }

void xdr::xBackup::parse_backup_list() noexcept
{
    this->backup_list.clear();
    for (const auto &entry : fs::directory_iterator(this->backup_path))
    {
        this->backup_list.push_back(entry.path());
    }
}

int xdr::xBackup::make_backup()
{
    if (xdr::make_backup(this->backup_path, this->X11_d, this->MDP_d) == XDR_ERR)
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
    for (const auto &_bpath : backup_list)
    {
        std::cout << XDR_PREF << _bpath << std::endl;
    }
}

void xdr::xBackup::load_config()
{
    std::ifstream input_file(fs::path(fs::path("/home") 
        / get_username() 
        / fs::path(".xdr_conf").string()));   
    
    if(input_file.is_open()){
        this->backup_path       = load_path(input_file);
        this->X11_d             = load_path(input_file); 
        this->MDP_d             = load_path(input_file);
        size_t backup_list_size;
        input_file.read(
            reinterpret_cast<char*>(&backup_list_size), sizeof(backup_list_size)
        );
        backup_list.resize(backup_list_size);
        for(size_t i = 0; i < backup_list_size; ++i){
            backup_list[i]      = load_path(input_file);
        }
        size_t username_size;
        input_file.read(
            reinterpret_cast<char*>(&username_size), sizeof(username_size)
        );
        username.resize(username_size);
        input_file.read(&username[0], username_size);
        input_file.close();
    }else{
        std::cerr << XDR_PREF << "Unable to open config binary to read!" << std::endl;
    }
}

void xdr::xBackup::save_conf()
{
    //попробуйте понять...
    std::ofstream out_file(
        fs::path(fs::path("/home") 
            / std::string(get_username()) 
            / fs::path(".xdr_conf")), 
        std::ios::binary);
    if (out_file.is_open())
    {
        save_path(out_file, this->backup_path);   // checkto
        save_path(out_file, this->X11_d);
        save_path(out_file, this->MDP_d);

        size_t backup_list_size = backup_list.size();
        out_file.write(reinterpret_cast<const char*>(&backup_list_size), sizeof(backup_list_size));
        for(const auto& path : backup_list){
            save_path(out_file, path);
        }
        size_t username_size = username.size();
        out_file.write(reinterpret_cast<const char*>(&username_size), sizeof(&username_size));
        out_file.write(username.c_str(), username_size);
        out_file.close();
        //
    }else{
        std::cerr << XDR_PREF << "Unable to open config binary! " << std::endl;
    }
}

void xdr::xBackup::save_path(std::ofstream& output_file, const fs::path& path){
    std::string path_str = path.string();
    size_t path_size= path_str.size();
    output_file.write(reinterpret_cast<const char*>(path_size), sizeof(path_size));
    output_file.write(path_str.c_str(), path_size);
}


fs::path xdr::xBackup::load_path(std::ifstream &input_file) const
{
    size_t path_size;
    input_file.read(reinterpret_cast<char*>(&path_size), sizeof(path_size));
    std::string path_str(path_size, '\0');
    input_file.read(&path_str[0], path_size);
    return fs::path(path_str);
}

int xdr::xUtiles::app(int argc, char *argv[])
{
    return 0;
}
