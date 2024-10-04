#include "xdrivers.hpp"

void xdr::xdr_handler(const std::exception &e, const std::string &add_info)
{
    std::cerr << "Error: " << e.what() << std::endl;
    if(!add_info.empty()){
        std::cerr << "Additional info: " << add_info << std::endl;
    }
}

