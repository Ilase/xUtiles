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

struct Driver {
        std::string version;
        std::string link;
        std::string release_date;
    };

    class xDriver
    {
    private:
        std::vector<Driver> drivers;
    public:
        std::vector<QString> graphicCardNames;
        std::vector<QString> driverNames;
        std::vector<QString> driverVersions;
        xDriver();
        std::vector<Driver>& getDrivers();
        std::vector<Driver>& getDrivers(std::string);
        std::string getLinkToVersion(std::string&);
        void downloadVersion(std::string&);
        std::string getVersionFileName(const std::string&);
    };

} //xdr namespace
#endif // XDRIVERS_HPP
