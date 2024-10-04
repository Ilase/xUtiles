#if !defined(XDRIVERS_HPP)
#define XDRIVERS_HPP
//
#include <iostream>
#include <stdexcept>
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

    };
}

#endif // XDRIVERS_HPP


