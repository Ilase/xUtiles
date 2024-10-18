#include "xutiles-drivers.hpp"
#include "xutiles.hpp"
xdr::xDriver::xDriver() {
    std::string output = exec("/opt/xUtils/parser");
    std::string line;
    auto stream = std::istringstream(output);
    while (getline(stream, line)) {
        this->versions.push_back(line);
    }
}

std::string xdr::xDriver::getLinkToVersion(std::string& version) {
    return std::string("https://us.download.nvidia.com/XFree86/Linux-x86_64/") + version + std::string("/NVIDIA-Linux-x86_64-") + version + std::string(".run");
}

void xdr::xDriver::downloadVersion(std::string& version) {
    std::string link = getLinkToVersion(version);
    //system("echo 1");
    system((std::string("wget ") + link).c_str());
}

std::vector<std::string>& xdr::xDriver::getVersions() {
    return this->versions;
}

//
//
