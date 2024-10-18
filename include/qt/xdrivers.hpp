#ifndef XDR
//
#define XDR // LIBRARY SPACE
#define XDR_PREF "[XDR] "
////
//--------------------------------------------------//------------------------------
#define XDR_ERR 11         // status codes
#define XDR_OK 10          //
                           //--------------------------------------------------//------------------------------
#if !defined(XDRIVERS_HPP) //
#define XDRIVERS_HPP       //
//----------------------------------------------//----------------------------------
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
#include <sstream>
    //----------------------------------------------//----------------------------------
    namespace fs = std::filesystem;                 //shortcut for std::filesystem
    //
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
    /*
    #include <X11/Xlib.h>
    #include <X11/extensions/Xrandr.h>
    */
    //
    namespace xdr
    {   
        void xdr_handler(const std::exception& e, const std::string& add_info);
        /*
        @brief Main app class whos make operations woth backups
        */
       class xDisplay
        {
        private:
        public:
            Display* display;
            Window root;
            int screenCount;
            Screen* defaultScreen;
            int selectedScreenId;
            Screen* selectedScreen;
            XRRScreenConfiguration* screenConfig;
            XRRScreenResources* screenResources;
            XRRScreenSize selectedScreenSize;
            int selectedScreenSizeId;
            std::vector<std::vector<XRRScreenSize>> screenSizes;
            std::vector<short> screenRates;
            std::string screenName;
            xDisplay();
            ~xDisplay();
            void ChangeResolution(int);
            void ChangeCurrentResolutionRates(int,short,Rotation);
            void SyncChanges();
            XRRScreenSize getCurrentResolution(Screen*);
            void getSelectedRates();
            void changeScreen(int);
            void changeMonitorPositions(int, int);
        };
       class xDriver
       {
       private:
           std::vector<std::string> versions;
       public:
           xDriver();
           std::vector<std::string>& getVersions();
           std::string getLinkToVersion(std::string&);
           void downloadVersion(std::string&);
       };
       class xBackup
       {
       private:
           fs::path backup_path = "/home/user";
           fs::path X11_d = "/etc/X11/";
           fs::path MDP_d = "/etc/modprobe.d/";
           std::vector<fs::path> backup_list;
           std::string username;

       public:
           xBackup(fs::path def_p = fs::path{});
           //~xDriver();
           //
           void parse_backup_list() noexcept;
           int make_backup();
           //
           std::vector<fs::path> get_backup_list();
           fs::path get_backup_path();
           fs::path get_X11_d();
           fs::path get_MDP_d();
           std::string get_username();
           void print_backup_list();
           void create_config();
           /// @brief Function to manage paths
           void save_path(std::ofstream& output_file, const fs::path& path);
           /// @brief Func for loading from paths files
           fs::path load_path(std::ifstream& input_file) const;
           void load_config();
           /// @brief Functionf for saving data in conf binary file
           void save_conf();


       };
       bool check_existing(const fs::path &p, fs::file_status s = fs::file_status{});
       // int repair_backup(fs::path &p);
       int make_backup(fs::path &ep, fs::path &x11_path, fs::path &mod_path);
       std::vector<fs::path> get_backup_list(fs::path &dp);
       int repair_backup(fs::path &bp);

       std::string trimer(const std::string& str);
       std::string get_username();


       class xUtiles{
       public:
           xdr::xDriver xDriver;
           xdr::xBackup xBackup;
       ///
       int app(int argc, char *argv[]);

       };
        void ChangeResolution(XRRScreenSize* screenSize);
        void SyncChanges();
        std::string GetGraphicDeviceName();
        void ChangeResolution(int, int, std::string &);
        std::pair<int, int> getResolution();
    }
    #endif // XDRIVERS_HPP
#endif // XDR
