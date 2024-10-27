#include "xutiles-include.hpp"

namespace xdr{
    class xConfigurator{
        std::vector<std::vector<std::string>> conf;
        fs::path conf_path = "/etc/X11/xorg.conf";
        public:
        xConfigurator(fs::path);
        int read_conf();
        void show_conf();
    };
}