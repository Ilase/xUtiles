#ifndef XUTILES_XORG_CONF_HPP
#define XUTILES_XORG_CONF_HPP
#include "xutiles-include.hpp"
namespace xdr
{
    const std::unordered_map<std::string, std::string> _input_flags ={
        {"Section" , "EndSection"},
        {"SubSection" , "EndSubSection"}
    };
    const std::unordered_map<std::string, std::vector<std::string>> _sections {
        {"Files", {}},
        {"ServerFlags", {}},
        {"Modules", {}},
        {"Extensions", {}}, 
        {"InputDevice", {}},
        {"InputClass", {}},
        {"Device", {}},
        {"VideoAdapter", {}},
        {"Monitor", {}},
        {"Modes", {}},
        {"Screen", {}},
        {"ServerLayout", {}},
        {"DRI", {}},
        {"Vendor", {}},
    };
    int make_conf();
    int edit_conf();
} // namespace xdr


#endif