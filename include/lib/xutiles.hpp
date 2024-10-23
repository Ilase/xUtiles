#ifndef XUTILES_HPP
#define XUTILES_HPP
#include "xutiles-drivers.hpp" 
#include "xutiles-include.hpp"
#include "xutiles-backup.hpp"

const std::map<
    std::string, 
    std::variant<
        std::function<void()>, 
        std::function<fs::path(xdr::xBackup&)>, 
        std::function<void(xdr::xBackup&)>,
        std::function<void(xdr::xBackup&, const std::string&)>
    >
> comand_map = {
    {
        "--help", 
        []() -> void {
            std::cout << XDR_PREF << "XDrive!"<< std::endl;
            std::cout << XDR_PREF << "Arguments: " << std::endl;
            std::cout << XDR_PREF << "--version \t -> show utile version" << std::endl;
            std::cout << XDR_PREF << "--backup  \t -> start backup process" << std::endl;
        }
    },
    {
        "--version",
        []() -> void {
            std::cout << XDR_PREF << "Version: 1.0" << std::endl;
        }
    },
    {
        "--backup",
        []() -> void {
            std::cout << XDR_PREF << "Start backup process" << std::endl;
        }
    },
    {
        "--backup-path",
        [](xdr::xBackup& _a) -> void {
            std::cout << XDR_PREF << _a.get_backup_path() << _a.get_username() << std::endl;
        }
    },
    {
        "--set-backup-path", 
        []() -> void {

        }
    },
    {
        "--backup-list",
        [](xdr::xBackup& _a) -> void {
            std::cout << XDR_PREF << "List of backups in directory: " << _a.get_backup_path() << std::endl;
        }
    },
    {
        "--repair-backup",
        [](xdr::xBackup& _a, const std::string& dir) -> void {
            std::cout << XDR_PREF << "Repairing backup from directory: " << dir << std::endl;
        }
    }
};

namespace xdr{
    std::string exec(const char*);

    class xUtiles{
        int argc;
        std::vector<std::string> args; 
    public:
        xUtiles(int argc, char* argv[]);
        int app();
    };
}



#endif
