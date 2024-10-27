
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <algorithm>

#define XDR_PREF "[XDR] "

namespace fs = std::filesystem;

namespace xdr{

    enum class _options{
        Identifier,
        Screen,
        InputDevice,
        ModulePath,
        FontPath,
        Load,
        Driver,
        Option,
        VendorName,
        ModelName,
        BusID,
        Viewport,
        Depth
    };

    enum class _tags{
        Section,
        EndSection,
        SubSection,
        EndSubSection
    };

    enum class _sections{
        ServerLayout,
        Module,
        Files,
        InputDevice,
        Monitor,
        Device,
        Screen
    };





    std::string options(_options s);
    std::string tags(_tags s);
    std::string sections(_sections s);



    class xConfigurator{
            std::vector<std::pair<int,std::vector<std::string>>> conf;
            fs::path conf_path = "/etc/X11/xorg.conf";
        public:
            xConfigurator(fs::path);
            int read_file();
            void show_conf();
            void show_conf(std::vector<std::vector<std::string>>);
            std::vector<std::pair<int,std::vector<std::string>>> 
                find_option(_options);
            void change_param(std::string, std::string, _options, _sections);
            int save_conf();
            
    };
}