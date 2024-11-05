#include "xutiles-include.hpp"
#include "xutiles.hpp"

namespace xdr{

    enum _vendor{
        Intel,
        Amd,
        Nvidia
    };

    class xConfigurator{
        _vendor vendor;
        public:
        xConfigurator(fs::path xp);
        int change_vsync();
        
        
    };
}