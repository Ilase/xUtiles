#include "xdrivers.hpp"


int main(int argc, char const *argv[])
{   
    int opt;
    bool display_opt;
    bool backup_opt;
    const char *_backup_folder_path = nullptr;
    std::vector<std::string> arguments(argv + 1, argv + argc); 
    //
    xdr::xDriver app;
    //
    std::map<std::string, std::function<void()>> comand_map = {
        {
            "--help", 
            [](){
                std::cout << XDR_PREF << "XDrive!"<< std::endl;
                std::cout << XDR_PREF << "Arguments: " << std::endl;
                std::cout << XDR_PREF << "--version \t -> show utile version" << std::endl;
                std::cout << XDR_PREF << "--backup  \t -> start backup process" << std::endl;
            }
        },
        {
            "--version",
            [](){
                std::cout << XDR_PREF << "Version: 1.0" << std::endl;
            }
        },
        {
            "--backup",
            [](){
                std::cout << XDR_PREF << "Start backup process" << std::endl;
            }
        },
        {
            "--backup-path",
            [](/*const auto& _app*/){
                // std::cout << XDR_PREF << app.get_backup_path() << std::endl;
            }
        }

    };


    for(const auto &arg : arguments){
        auto it = comand_map.find(arg);
        if (it !=comand_map.end()){
            it->second();
        }else{
            std::cout << XDR_PREF << "Unknown argument: " << arg << std::endl; 
        }
    } 


    return XDR_OK;
}
