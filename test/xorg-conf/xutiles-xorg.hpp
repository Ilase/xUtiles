
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <algorithm>
#include <optional>
#include <unordered_map>

#define XDR_PREF "[XDR] "

namespace fs = std::filesystem;

namespace xdr{

    enum _type{
        NVIDIA,
        AMD,
        INTEL
    };

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
        Depth,
        NOWAY
    };

    enum class _tags{
        Section,
        EndSection,
        SubSection,
        EndSubSection,
        NOWAY
    };

    enum class _sections{
        ServerLayout,
        Module,
        Files,
        InputDevice,
        Monitor,
        Device,
        Screen,
        NOWAY
    };  

    

    std::string options(_options s);
    std::string tags(_tags s);
    std::string sections(_sections s);

    //
    struct line_table{
        size_t n;
        std::optional<_sections> sec;
        //std::optional<_tags> tag;
        std::optional<_options> opt;
        std::vector<std::string> line;
    };
    //
    class textEditor{
            std::vector<std::vector<std::string>> text;
            /// @brief thiss sheet is cumming up and blow my jobS
            std::vector<xdr::line_table> tag_table;
            fs::path path = "/etc/X11/xorg.conf.d/100-xutils-xorg.conf";
            //std::vector<size_t> device_pos;
        public:
            //----------------------------------------------------
            textEditor();    
            textEditor(fs::path);
            //----------------------------------------------------
            // File 
        
            virtual int create();
            virtual int show();
            virtual int load();
            // Text
            virtual bool is_word_exist(std::string);
            virtual int insert_line(std::vector<std::string>);
            virtual int delete_line(size_t); 
            virtual int add_line_in_tag_table(const xdr::line_table&);



            //std::vector<std::vector<std::string>> generate_driver_section(_sections, _type);
            //std::vector<std::vector<std::string>> generate_screen_section(_sections);
            //bool is_word_exist(std::string);

            //
            // int load();
            // //void show();
            // void show(std::vector<std::vector<std::string>>);
            // std::vector<std::vector<std::string>> 
            //     find_option(_options);
            // void change_param(std::string, std::string, _options, _sections);
            // int save_conf();
            // int insert_line(size_t, std::vector<std::string>);    
            // int delete_line(int);
            // //
            // int change_tearing(size_t, bool);
            // //
            // std::vector<std::string> generate_option(_options,std::vector<std::string>);
    
    };


    class xConf : public textEditor{
        public: 
        
    };

    _sections check_sec(std::string&);
    _options check_opt(std::string&);
    int reload_config();
}