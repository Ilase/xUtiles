#include "xutiles-drivers.hpp"
#include "xutiles.hpp"
xdr::xDriver::xDriver() {
    QString graphicDeviceOutput = xdr::exec("inxi -G").c_str();
    //QRegExp devicePatern(R"(Device-\d+: (\w+(?: [\w\[\]_-]+)+) \w+:)");
    QRegularExpression devicePatern(R"(Device-\d+: (\w+(?: [\w\[\]_-]+)+) driver: (\w+) v: ([a-zA-Z]+|\d+(?:\.\d+)+))");
    QRegularExpressionMatchIterator mathces = devicePatern.globalMatch(graphicDeviceOutput);
    while (mathces.hasNext()) {
        QRegularExpressionMatch match = mathces.next();
        if (match.hasMatch()) {
            this->graphicCardNames.push_back(match.captured(1));
            this->driverNames.push_back(match.captured(2));
            this->driverVersions.push_back(match.captured(3));
        }
    }

    std::string parserOutput = xdr::exec("/opt/xUtils/parser");
    std::string line;
    auto stream = std::istringstream(parserOutput);
    while (getline(stream, line)) {
        this->drivers.push_back(xdr::Driver {line, "", ""});
    }
}

std::string xdr::xDriver::getLinkToVersion(std::string& version) {
    return std::string("https://us.download.nvidia.com/XFree86/Linux-x86_64/") + version + std::string("/NVIDIA-Linux-x86_64-") + version + std::string(".run");
}

void xdr::xDriver::downloadVersion(std::string& version) {
    std::string link = getLinkToVersion(version);
    system((std::string("wget ") + link).c_str());
}

std::vector<xdr::Driver>& xdr::xDriver::getDrivers() {
    return this->drivers;
}

std::vector<xdr::Driver>& xdr::xDriver::getDrivers(std::string name) {
    this->drivers.clear();
    std::string parserOutput = xdr::exec(("/opt/xUtils/parser_driver --name '" + name + "'").c_str());
    std::string line;
    auto stream = std::istringstream(parserOutput);
    while (getline(stream, line)) {
        QRegularExpression r(R"((.*),(.*),(.*))");
        this->drivers.push_back(xdr::Driver{r.match(line.c_str()).captured(1).toStdString(),
                                 r.match(line.c_str()).captured(2).toStdString(),
                                 r.match(line.c_str()).captured(3).toStdString()});
    }
    return this->drivers;
}

std::string xdr::xDriver::getVersionFileName(const std::string &version) {
    return std::string("NVIDIA-Linux-x86_64-") + version + std::string(".run");
}

std::string xdr::driverFolderName()  {
    return "/home/" + std::string(xdr::get_username().c_str()) + "/.cache/xdrivers/";
}

//
//
