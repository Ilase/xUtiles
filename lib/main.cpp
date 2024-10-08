#include "xdrivers.hpp"


int main(int argc, char const *argv[])
{   
    if(argc < 2){
        std::cerr << "Usage " << argv[0] << " <option>" << std::endl;
        return 1;
    }
    //
    xdr::xDriver app = xdr::xDriver();
    //
    switch (argv[1][0]){
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
    return XDR_OK;
}
