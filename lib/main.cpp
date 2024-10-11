#include "xdrivers.hpp"

#define XDR_DEBUG

int main(int argc, char const *argv[])
{   
    if(argc < 2){
        std::cerr << XDR_PREF << "No comand provided" << std::endl;
    }
    int opt;
    bool display_opt;
    bool backup_opt;
    //const char *_backup_folder_path = nullptr;
    std::vector<std::string> arguments(argv + 1, argv + argc); 
    // Call constructor of main class
    xdr::xBackup app;
    //
    
    //

    std::map<
        std::string, 
        std::variant<
            std::function<void()>, 
            std::function<fs::path(xdr::xBackup&)>, 
            std::function<void(xdr::xBackup&)>,
            std::function<void(xdr::xBackup&, const std::string&)>
        >
    > 
    /*
    @brief List 1 ot 1 of arguments functions.
    @author Ilase
    */
    comand_map = {
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
            [](xdr::xBackup& _a, const std::string&) -> void {
                std::cout << XDR_PREF << "Repairing backup from directory: " << std::endl;
            }
        }
    };


    for(size_t i = 0; i < arguments.size() ; ++i){
        const auto& arg = arguments[i];
        auto it = comand_map.find(arg);
        if (it !=comand_map.end()){ 
            std::visit([&app, &arguments, &i](auto&& func){
                using func_type = std::decay_t<decltype(func)>;
                if constexpr (std::is_invocable_v<func_type, xdr::xBackup&>){
                    func(app);   
                } else if constexpr (std::is_invocable_v<func_type, xdr::xBackup&, const std::string&>){
                    if(i + 1 < arguments.size()){
                        func(app, arguments[i + 1]);
                        ++i;
                    } else {
                        std::cerr << XDR_PREF << "Error: Missing path arg." << arguments[i] << std::endl; 
                    }
                }else {
                    func();
                }
            }, it->second);
        }else{
            std::cout << XDR_PREF << "Unknown argument: " << arg << std::endl; 
        }
    } 


    return XDR_OK;
}
