#include "xutiles-xorg.hpp"


std::string xdr::options(_options s) {
    switch(s){
        case _options::Identifier: return "ServerLayout";
        case _options::Screen: return "Screen";
        case _options::InputDevice: return "InputDevice";
        case _options::ModulePath: return "ModulePath";
        case _options::FontPath: return "FontPath";
        case _options::Load: return "Load";
        case _options::Driver: return "Driver";
        case _options::VendorName: return "Option";
        case _options::ModelName: return "VendorName";
        case _options::BusID: return "BusID";
        case _options::Viewport: return "Viewport";
        case _options::Depth: return "Depth";
        default: throw std::invalid_argument("Unimpliment item");
    }
}

std::string xdr::tags(_tags s) {
    switch(s){
        case _tags::Section: return "\"Section\"";
        case _tags::EndSection: return "\"EndSection\"";
        case _tags::SubSection: return "\"SubSection\"";
        case _tags::EndSubSection: return "\"EndSubSection\"";
        default: throw std::invalid_argument("Unimpliment item");
    }
}

std::string xdr::sections(_sections s) {
    switch(s){
        case _sections::ServerLayout: return "\"ServerLayout\"";
        case _sections::Module: return "\"Module\"";
        case _sections::Files: return "\"Module\"";
        case _sections::InputDevice: return "\"InputDevice\"";
        case _sections::Monitor: return "\"Monitor\"";
        case _sections::Device: return "\"Device\"";
        case _sections::Screen: return "\"Screen\"";
        default: throw std::invalid_argument("Unimpliment item");
    }
}





// int xdr::reload_config()
// {
    
//     return 0;
// }

// xdr::xConfigurator::xConfigurator(fs::path xorgcp)
// {
//     if(xorgcp != this->conf_path){
//         if(!fs::exists(xorgcp)){
//             std::cerr << XDR_PREF << "Xorg path doesn't exists\n";
//         }
//         this->conf_path = xorgcp;
//     }
// }

// std::vector<std::vector<std::string>> xdr::xConfigurator::generate_driver_section(_sections, _type)
// {
//     std::vector<std::vector<std::string>> result;

//     return result;
// }

// bool xdr::xConfigurator::is_word_exist(std::string querry)
// {
//     for(auto const& line : this->conf){
//         for(const auto& word : line){
//             if(word == querry){
//                 return true;
//             }
//         }
//     }
//     return false;
// }

// int xdr::xConfigurator::load()
// {
//     std::ifstream input(this->conf_path);
//     if(!input.is_open()){
//         std::cerr << XDR_PREF << "Unable to open file for read\n";
//         return 1;
//     }
//     int line_num;
//     std::string line;
//     bool is_device;
//     while(std::getline(input,line)){
//         line_num++;
//         std::istringstream iss(line);
//         std::vector<std::string> line_words;
//         std::string word;
//         while(iss >> word){
//             line_words.push_back(word);
            
//         }
//         this->conf.push_back(line_words);
//     }

//     return 0;
// }

// void xdr::xConfigurator::show()
// {
    
//     for(size_t l = 0; l < this->conf.size(); ++l){
//         // for(const auto& word : line.second()){
//         //     std::cout << word << " ";
//         // }
        
        
//         for(size_t w = 0; w < this->conf[l].size(); ++w){
//             std::cout << this->conf[l][w] << " ";
//         }

//         std::cout << "\n";
//     }
// }

// void xdr::xConfigurator::show(std::vector<std::vector<std::string>> _ivec)
// {
//     for(const auto& line : _ivec){
//         for(const auto& word : line){
//             std::cout << word << " ";
//         }
//         std::cout << "\n";
//     }    
// }

// std::vector<std::vector<std::string>>
//     xdr::xConfigurator::find_option(_options opt)
// {
//     std::vector<std::vector<std::string>> result;
//     std::string opt_str = xdr::options(opt);
//     for(const auto& line : this->conf){
//         for(const auto& word : line){
//             if(word == opt_str){
//                 result.push_back(line); 
//             }
//         }
//         // if(!result.empty()){
//         //     break;
//         // }
//     }    
//     return result;
// }



// int xdr::xConfigurator::save_conf()
// {
//     if(fs::exists(this->conf_path)){
//         std::cout << XDR_PREF << "Updating xorg.conf. Removing/n";
//         fs::remove(this->conf_path);
//     }
//     //!!!!! 
//     std::ofstream output(this->conf_path);

//     if(!output.is_open()){
//         std::cerr << XDR_PREF << "Unable open output path/";
//     } else {
//         for(const auto& line : this->conf){
//             for(const auto& word : line){
//                 output << word << " ";
//             }
//             output << "\n";
//         }
//     }


//     return 0;
// }

// int xdr::xConfigurator::insert_line(size_t pos, std::vector<std::string> line)
// {   
//     auto it = this->conf.begin();
//     std::advance(it, pos);
//     this->conf.insert(it, line);
//     return 0;
// }

// int xdr::xConfigurator::delete_line(int pos)
// {
//     this->conf.erase(this->conf.begin() + pos);
//     return 0;
// }

// int xdr::xConfigurator::change_tearing(size_t device, bool state)
// {
//     std::vector<std::vector<std::string>> conf_buff;
//     auto it = this->conf.begin();
//     std::advance(it, device + 1 );
    
//     for(const auto it : this->conf){
        
//         if(it[0] != "EndSection"){
//             conf_buff.push_back(it);
//         }
//     }
//     it = conf_buff.end();
//     std::advance(it, conf_buff.end() - 1);
    
    
//     {
//         if(state){
//             conf_buff.insert(conf_buff.end() - 1 ,generate_option(_options::Option, {"\"TearFree\"", "\"true\""}));
//         } else {
//             conf_buff.insert(conf_buff.end() - 1 ,generate_option(_options::Option, {"\"TearFree\"", "\"true\""}));
//         }
//     }
//     return 0;
// }

// std::vector<std::string> xdr::xConfigurator::generate_option(_options opt, std::vector<std::string> params)
// {
//     std::vector<std::string> result;
//     result.push_back("Option");
//     auto it = params.begin();
//     std::advance(it, 1);
//     for(const auto& it : params){
//         result.push_back(it);
//     }

//     return result;
// }

int xdr::textEditor::create()
{
    std::ofstream buff_file(this->path);
    if(buff_file.is_open()){
        buff_file.close();
        std::cout << XDR_PREF << "File " << this->path.filename() << " created succsessfuly";
        return 0;
    } else {
        std::cerr << XDR_PREF << "Can't create file!\n";
        return 1;
    }
}

int xdr::textEditor::show()
{
    for(const auto& line : this->text){
        for(const auto& word : line){
            std::cout << word << " ";
        }
        std::cout << "\n";
    }
    return 0;
}

int xdr::textEditor::load()
{   
    std::ifstream input(this->path);
    if(!input.is_open()){
        std::cerr << XDR_PREF << "Unable open file for reading.\n"; 
        return 1;
    }

    size_t line_pos = 0;
    std::string buff_line;
    //
    _sections curr_sec;
    _options opt_var;
    //
    while (std::getline(input, buff_line))
    {
        std::istringstream iss(buff_line);
        std::vector<std::string> word_line;
        std::string word;
        //check this out nigga
        while(iss >> word){
            word_line.push_back(word);
            //

            //
        }
        this->text.push_back(word_line);
        //this->tag_table.push_back({line_pos, check_sec(word), check_sec(word), word_line});
        add_line_in_tag_table({line_pos, check_sec(word), check_opt(word), word_line});
        line_pos++;
    }
    
    
    return 0;
}

int xdr::textEditor::add_line_in_tag_table(const xdr::line_table & line)
{
    this->tag_table.push_back(line);
}

xdr::_sections xdr::check_sec(std::string& word)
{
    for(size_t i = 0; i < static_cast<int>(_sections::NOWAY); ++i){
        std::string _buff = sections(static_cast<_sections>(i));
        if(word == _buff){
            return static_cast<_sections>(i);
        }
    }
    return _sections::NOWAY;
}

xdr::_options xdr::check_opt(std::string& word)
{
    for(size_t i = 0; i < static_cast<int>(_options::NOWAY); ++i){
        std::string _buff = options(static_cast<_options>(i));
        if(word == _buff){
            return static_cast<_options>(i);
        }
    }
    return _options::NOWAY;
}


