#include "xutiles-xorg.hpp"

xdr::xConfigurator::xConfigurator(fs::path xp){

    std::string querry = std::string(xdr::exec("lspci -nnn"));
    std::vector<std::string> adaptors;
    for(const auto& word : querry){
        size_t found = querry.find(word);
        if(found != std::string::npos){
            this->vendor = ;
        }    
    }


}

int xdr::xConfigurator::change_vsync()
{
    
    return 0;
}
