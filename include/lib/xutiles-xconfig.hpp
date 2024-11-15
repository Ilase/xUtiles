#include <utility>
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <array>
#include <memory>
#include <stdexcept>
namespace xdr{
    enum graphic_type{
        AMD,
        INTEL,
        NVIDIA
    };
    class xConfig{
        graphic_type type;
        std::vector<std::string> devices;
        public:
        xConfig();
        void check_type();
        std::vector<std::string> get_monitor_names();
    };
    int change_tearing(bool, std::string, graphic_type);
    std::vector<std::string> get_adapt();
    
    int apply_changes();
};
