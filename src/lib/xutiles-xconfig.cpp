#include "xutiles-xconfig.hpp"

xdr::xConfig::xConfig()
{
    this->devices = get_monitor_names();
}

inline void xdr::xConfig::check_type()
{

}

int xdr::change_tearing(bool opt, std::string monitor_name, graphic_type _type)
{
    std::string wha = opt ? "on" : "off";
    const char* comand;
    switch (_type){
        case graphic_type::INTEL:
            comand = ("xrandr --output " + monitor_name + " --set TearFree " + wha).c_str();
        break;
        case graphic_type::AMD:
            comand = ("xrandr --output " + monitor_name + " --set TearFree " + wha).c_str();
            break;
        case graphic_type::NVIDIA:
            break;
        default:
            std::cerr << "Not valid graphical vendor\n";
    }


    system(comand);
    return 0;
}

std::vector<std::string> xdr::get_adapt()
{   

    std::array<char, 128> buffer;
    std::string result;
    std::vector<std::string> output;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("lspci -k | grep -E \"VGA|3D\" | grep -E \"NVIDIA|AMD|Intel\"", "r"), pclose);

    if(!pipe){
        throw std::runtime_error("Can't execute comand");
    }
    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr){
        result += buffer.data();
        output.push_back(buffer.data());
    }
    return output;
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
