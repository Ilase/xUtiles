#if !defined(XDRIVERS_HPP)
#define XDRIVERS_HPP
//
#include <iostream>
#include <stdexcept>
#include <utility>
//
#include <X11/Xlib.h>
#include <GL/glx.h>
//
namespace xdr
{
    void xdr_handler(const std::exception& e, const std::string& add_info);
    class XDriver {

    public:
        XDriver();

        void getSettings();
        std::pair<int, int> getResolution();
    };
}

#endif // XDRIVERS_HPP


