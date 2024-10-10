#include "xdrivers.hpp"


int main(int argc, char const *argv[])
{   
    int opt;
    bool display_opt;
    bool backup_opt;
    const char *_backup_folder_path = nullptr;
    //while(true);
    //
    xdr::xDriver app = xdr::xDriver();
    //
    if(argv[1] != NULL){
        switch (argv[1][1]){ // catch the line
            case 'b':
                std::cout << "Option 'backup' selected." << std::endl;
                break;
            case 'd':
                std::cout << "Option 'display' selected" << std::endl;
                break;
            default:
                std::cerr << "Unknown option: " << argv[1] << std::endl;
                return XDR_ERR;
                break;
        }
    }
    return XDR_OK;
}
