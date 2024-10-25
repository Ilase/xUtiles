#include "xutiles-drivers.hpp"
#include "xutiles.hpp"
xdr::xDriver::xDriver() {
    QRegExp versionPatern("v: (\\d+(?:\\.\\d+)+)");
    QString graphicDeviceOutput = xdr::exec("inxi -G").c_str();
    if (graphicDeviceOutput.contains(versionPatern)){
        //QStringList captured = versionPatern.capturedTexts();
        this->driverVersion = versionPatern.cap(1);
    }
    QRegExp devicePatern(R"(Device-\d+: (\w+(?: [\w\[\]_-]+)+) \w+:)");


    if (graphicDeviceOutput.contains(devicePatern)) {
        this->graphicCardName = devicePatern.cap(1);
    }
    QRegExp driverNamePatern("driver: (\\w+)");
    if (graphicDeviceOutput.contains(driverNamePatern)) {
        this->driverName = driverNamePatern.cap(1);
    }
    //std::cout <<  driverName.toStdString() << '\t' << driverVersion.toStdString() << '\t' << graphicCardName.toStdString() << '\n';
    std::string parserOutput = xdr::exec("/opt/xUtils/parser");
    std::string line;
    auto stream = std::istringstream(parserOutput);
    while (getline(stream, line)) {
        this->versions.push_back(line);
    }
}

std::string xdr::xDriver::getLinkToVersion(std::string& version) {
    return std::string("https://us.download.nvidia.com/XFree86/Linux-x86_64/") + version + std::string("/NVIDIA-Linux-x86_64-") + version + std::string(".run");
}

void xdr::xDriver::downloadVersion(std::string& version) {
    std::string link = getLinkToVersion(version);
    system((std::string("wget ") + link).c_str());
}

std::vector<std::string>& xdr::xDriver::getVersions() {
    return this->versions;
}

std::string xdr::xDriver::getVersionFileName(const std::string &version) {
    return std::string("NVIDIA-Linux-x86_64-") + version + std::string(".run");
}

std::string xdr::driverFolderName()  {
    return "/home/" + std::string(xdr::get_username().c_str()) + "/.cache/xdrivers/";
}

//
//
