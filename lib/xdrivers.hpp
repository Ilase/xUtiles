#if !defined(XDRIVERS_HPP)
#define XDRIVERS_HPP
//
#include <iostream>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <filesystem>
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

    void backup();
    void backup(char *args[]);
}

#endif // XDRIVERS_HPP


