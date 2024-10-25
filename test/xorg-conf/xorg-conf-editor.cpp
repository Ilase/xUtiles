#include "xorg-conf-editor.hpp"

xdr::xConf::xConf()
{
    try{
    std::cout << "Check exists: " << !std::filesystem::exists(this->xorg_conf);
    }
    catch(std::filesystem::filesystem_error const&fe){
        std::cerr << "Err:" << fe.what() << std::endl;
    }
    
}
