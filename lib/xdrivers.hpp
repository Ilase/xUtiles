#ifndef XDR
//
#define XDR
//
#define XDR_ERR 11
#define XDR_OK 10
//
    #if !defined(XDRIVERS_HPP)
    #define XDRIVERS_HPP
    //
    #include <iostream>
    #include <stdexcept>
    #include <utility>
    #include <algorithm>
    #include <filesystem>
    #include <string_view>
    //
    namespace fs = std::filesystem;
    //
    #include <X11/Xlib.h>
    #include <GL/glx.h>
    //
    namespace xdr
    {
        void xdr_handler(const std::exception& e, const std::string& add_info);
        std::pair<int, int> getResolution();
        bool check_existing(const fs::path& p, fs::file_status s = fs::file_status{});
        int backup();
        int backup(char *args[]);
    }
    #endif // XDRIVERS_HPP
#endif // XDR