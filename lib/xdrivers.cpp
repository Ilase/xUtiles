#include "xdrivers.hpp"

void xdr::xdr_handler(const std::exception &e, const std::string &add_info)
{
    std::cerr << "Error: " << e.what() << std::endl;
    if(!add_info.empty()){
        std::cerr << "Additional info: " << add_info << std::endl;
    }
}

std::pair<int, int> xdr::getResolution() {
    Display* display = XOpenDisplay(NULL);
    return std::pair<int, int>(XDisplayWidth(display,0), XDisplayHeight(display,0));
}
