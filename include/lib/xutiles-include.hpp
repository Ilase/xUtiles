#ifndef XUTILES_INCLUDE_HPP
#define XUTILES_INCLUDE_HPP

#define XDR_PREF "[XDR] "

//--------------------------------------------------//
#define XDR_ERR 11                                  // status codes
#define XDR_OK 10                                   //
//--------------------------------------------------//
#include <iostream>         // basic input/output stream
#include <stdexcept>        // exception class for handler
#include <utility>          // std containers
#include <algorithm>        // std functions
#include <filesystem>       // lib for working with filesystem
#include <string_view>      // view strings in fs iteration
#include <vector>           // paths container
#include <chrono>           //
#include <ctime>            //
#include <iomanip>          //
#include <map>              // for argiments in main() 
#include <functional>       // for arguments in main()
#include <any>              // for any type argument in comand_map
#include <variant>          // for selectcting lambda functions
#include <pwd.h>            //
#include <unistd.h>         //
#include <cstring>          //
#include <fstream>          // config reade/write includes
#include <sstream>          //
extern "C" {
    #include <X11/Xlib.h>
    #include <X11/extensions/Xrandr.h>
    #undef Bool
    #undef CursorShape
    #undef Expose
    #undef KeyPress
    #undef KeyRelease
    #undef FocusIn
    #undef FocusOut
    #undef FontChange
    #undef None
    #undef Status
    #undef Unsorted

}
//--------------------------------------------------//
namespace fs = std::filesystem;                     // shortcut for std::filesystem
//--------------------------------------------------//


namespace xdr{



};


#endif
