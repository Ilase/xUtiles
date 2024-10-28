
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
            std::vector<std::vector<std::string>> conf;
            fs::path conf_path = "/etc/X11/xorg.conf";
            std::vector<size_t> device_pos;
        public:
            xConfigurator(fs::path);
            int read_conf();
            void show_conf();
            void show_conf(std::vector<std::vector<std::string>>);
            std::vector<std::vector<std::string>> 
                find_option(_options);
            void change_param(std::string, std::string, _options, _sections);
            int save_conf();
            int insert_line(size_t, std::vector<std::string>);    
            int delete_line(int);
            //
            int change_tearing(size_t, bool);
            //
            std::vector<std::string> generate_option(_options,std::vector<std::string>);
    
    };

    int reload_config();
}