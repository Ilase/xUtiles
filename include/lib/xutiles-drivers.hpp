//-------------------------//-----------------------//------------------------------
#ifndef XDRIVERS_HPP //
#define XDRIVERS_HPP       //
//--------------------------------------------------//------------------------------
#include <iostream>         // basic input/output stream
#include <string>
#include <vector>
#include <QRegularExpression>
#include "xutiles-include.hpp"
//
namespace xdr
{   


    class xDriver
    {
    private:
        std::vector<std::string> versions;
    public:
        std::vector<QString> graphicCardNames;
        std::vector<QString> driverNames;
        std::vector<QString> driverVersions;
        xDriver();
        std::vector<std::string>& getVersions();
        std::string getLinkToVersion(std::string&);
        void downloadVersion(std::string&);
        std::string getVersionFileName(const std::string&);
    };

} //xdr namespace
#endif // XDRIVERS_HPP
