//-------------------------//-----------------------//------------------------------
#if !defined(XDRIVERS_HPP) //
#define XDRIVERS_HPP       //
//--------------------------------------------------//------------------------------
#include <iostream>         // basic input/output stream
#include <string>
#include <vector>
//
namespace xdr
{   

    class xDriver
    {
    private:
        std::vector<std::string> versions;
    public:
        xDriver();
        std::vector<std::string>& getVersions();
        std::string getLinkToVersion(std::string&);
        void downloadVersion(std::string&);
        std::string getVersionFileName(std::string&);
    };

} //xdr namespace
#endif // XDRIVERS_HPP
