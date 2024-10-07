#ifndef XDR
//
#define XDR                                         //LIBRARY SPACE
////
//--------------------------------------------------//------------------------------
#define XDR_ERR 11                                  //status codes
#define XDR_OK 10                                   //
//--------------------------------------------------//------------------------------
    #if !defined(XDRIVERS_HPP)                      //
    #define XDRIVERS_HPP                            //
    //----------------------------------------------//----------------------------------
    #include <iostream>                             //basic input/output stream
    #include <stdexcept>                            //exception class for handler
    #include <utility>                              //std containers
    #include <algorithm>                            //std functions
    #include <filesystem>                           //lib for working with filesystem
    #include <string_view>                          //view strings in fs iteration 
    #include <vector>                               // paths container 
    //----------------------------------------------//----------------------------------
    namespace fs = std::filesystem;                 //shortcut for std::filesystem
    //
    #include <X11/Xlib.h>
    #include <GL/glx.h>
    //
    namespace xdr
    {   
        void xdr_handler(const std::exception& e, const std::string& add_info);
        /*
        @brief Main app class whos make operations woth backups
        */
        class xDriver {
            private:
                fs::path backup_path = "/mnt/xdr-backups";
                fs::path X11_d = "/etc/X11/";
                fs::path MDP_d = "/etc/modprobe.d/";
                std::vector<fs::path> backups_list;
            public:
                xDriver(fs::path def_p = fs::path{});
                //
                void parse_backup_list();
                //
                //friend int repair_backup();
                //friend int make_backup();
        };

        std::pair<int, int> getResolution();
        bool check_existing(const fs::path& p, fs::file_status s = fs::file_status{});
        int repair_backup(fs::path &p);
        int make_backup(fs::path &ep);
    }
    #endif // XDRIVERS_HPP
#endif // XDR