#include "xutiles-include.hpp"

namespace xdr{
    class textEditor{
        std::vector<std::vector<std::string>> text;
        fs::path path = "/etc/X11/xorg.conf";
        public:
        textEditor(fs::path);
        int load();
        void show();
    };
}