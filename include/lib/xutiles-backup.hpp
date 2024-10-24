#ifndef _XUTILES_BACKUP_HPP
#define _XUTILES_BACKUP_HPP
#include "xutiles-include.hpp"

namespace xdr {

class xBackup
    {
    private:
        fs::path backup_path = "/home/user";
        fs::path X11_d = "/etc/X11/";
        fs::path MDP_d = "/etc/modprobe.d/";
        std::vector<fs::path> backup_list;
        std::string username;

    public:
        xBackup(fs::path def_p = fs::path{});
        //~xDriver();
        //
        void parse_backup_list() noexcept;
        int make_backup();
        //
        std::vector<fs::path> get_backup_list();
        fs::path get_backup_path();
        fs::path get_X11_d();
        fs::path get_MDP_d();
        std::string get_username();
        void print_backup_list();
        void create_config();
        /// @brief Function to manage paths 
        void save_path(std::ofstream& output_file, const fs::path& path);
        /// @brief Func for loading from paths files
        fs::path load_path(std::ifstream& input_file) const;
        void load_config();
        /// @brief Functionf for saving data in conf binary file
        void save_conf();


    };
    bool check_existing(const fs::path &p, fs::file_status s = fs::file_status{});
    // int repair_backup(fs::path &p);
    int make_backup(fs::path &ep, fs::path &x11_path, fs::path &mod_path);
    std::vector<fs::path> get_backup_list(fs::path &dp);
    int repair_backup(fs::path &bp);

    std::string trimer(const std::string& str);
    std::string get_username();
    int make_download_folder();
};

#endif
