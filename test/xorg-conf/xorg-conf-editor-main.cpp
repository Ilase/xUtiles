#include "xutiles-xconfig.hpp"

int main(int argc, char *argv[]){
    xdr::xConfig app;
    std::vector<std::string> mon;
    mon = xdr::get_adapt("lspci -k | grep -A 2 -E \"(VGA|3D)\"");

    for(const auto& line : mon){
        std::cout << line << std::endl;
    }
}