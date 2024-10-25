//-------------------------//-----------------------//------------------------------
#ifndef XDRIVERS_HPP //
#define XDRIVERS_HPP       //
//--------------------------------------------------//------------------------------
#include <iostream>         // basic input/output stream
#include <string>
#include <vector>
#include <QRegExp>
#include "xutiles-include.hpp"
//
namespace xdr
{   


    class xDriver
    {
    private:
        std::vector<std::string> versions;
    public:
        QString graphicCardName;
        QString driverName;
        QString driverVersion;
        xDriver();
        std::vector<std::string>& getVersions();
        std::string getLinkToVersion(std::string&);
        void downloadVersion(std::string&);
        std::string getVersionFileName(const std::string&);
    };

} //xdr namespace
#endif // XDRIVERS_HPP
