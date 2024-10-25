#include "xutiles-include.hpp"

namespace xdr{
    class xConfigurator{

        public:
        xConfigurator(fs::path xp);
        int section_read();
        int section_write();
        int main_read();
        int main_write();
    };
}