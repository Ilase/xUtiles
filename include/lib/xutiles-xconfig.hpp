#include <utility>
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <cstdlib>
namespace xdr{
    
    class xConfig{
        std::vector<std::string> devices;
        public:
        xConfig();
        int change_tearing(bool, std::string);
        std::vector<std::string> get_monitor_names();
    };

};