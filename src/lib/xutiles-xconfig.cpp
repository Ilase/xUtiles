#include "xutiles-xconfig.hpp"

xdr::xConfig::xConfig()
{
    this->devices = get_monitor_names();
}

int xdr::xConfig::change_tearing(bool opt, std::string monitor_name)
{
    std::string wha = opt ? "on" : "off";
    const char* comand = ("xrandr --output " + monitor_name + " --set TearFree " + wha).c_str();
    system(comand);
    return 0;
}

std::vector<std::string> xdr::xConfig::get_monitor_names()
{ 
    std::vector<std::string> names;
    FILE* pipe = popen("xrandr --listmonitors | awk '/^ / {print $4}'", "r");
    if (!pipe) return names;
    int fd = fileno(pipe);
    FILE* file = fdopen(fd, "r");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != nullptr) {
        names.emplace_back(buffer);
    }
    pclose(pipe);
    return names;
}
