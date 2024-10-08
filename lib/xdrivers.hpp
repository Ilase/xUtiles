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
    #include <chrono>                               //
    #include <ctime>                                //
    #include <iomanip>
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
       class xDriver
        {
        private:
        public:
            Display* display;
            Window root;
            int screenCount;
            Screen* defaultScreen;
            XRRScreenConfiguration* screenConfig;
            XRRScreenResources* screenResources;
            std::vector<std::vector<XRRScreenSize>> screenSizes;
            xDriver();
            ~xDriver();
            void ChangeResolution(XRRScreenSize* screenSize);
            void SyncChanges();
        };
        class xDriver {
            private:
                fs::path backup_path = "/mnt/xdr-backups/";
                fs::path X11_d = "/etc/X11/";
                fs::path MDP_d = "/etc/modprobe.d/";
                std::vector<fs::path> backup_list;
            public:
                xDriver(fs::path def_p = fs::path{});
                ~xDriver();
                //
                void parse_backup_list();
                int make_backup();
                //
                fs::path get_backup_path();
                fs::path get_X11_d();
                fs::path get_MDP_d();
                //friend int repair_backup();
                //friend int make_backup();
        };

        std::pair<int, int> getResolution();
        bool check_existing(const fs::path& p, fs::file_status s = fs::file_status{});
        int repair_backup(fs::path &p);
        int make_backup(fs::path &ep, fs::path &x11_path, fs::path &mod_path);
    }
    #endif // XDRIVERS_HPP
#endif // XDR