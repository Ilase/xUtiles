#include "xutiles-backup.hpp"

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
// Getters/Setters

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


std::string xdr::get_username()
{
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    return pw->pw_name;
}

int xdr::make_download_folder()
{
    fs::path download_path = "/opt/xUtiles/drivers/";
    try{
    fs::create_directories(download_path);
    }catch(std::exception &e){
        std::cerr << XDR_PREF << "Error:" << e.what() << std::endl;
        return XDR_ERR;   
    }
    return XDR_OK;
}

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

    }

    return XDR_OK;
}

