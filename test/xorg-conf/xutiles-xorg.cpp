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


xdr::xConfigurator::xConfigurator(fs::path xorgcp)
{
    if(xorgcp != this->conf_path){
        if(!fs::exists(xorgcp)){
            std::cerr << XDR_PREF << "Xorg path doesn't exists\n";
        }
        this->conf_path = xorgcp;
    }
}

int xdr::xConfigurator::read_file()
{
    std::ifstream input(this->conf_path);
    if(!input.is_open()){
        std::cerr << XDR_PREF << "Unable to open file for read\n";
        return 1;
    }
    int line_num;
    std::string line;
    while(std::getline(input,line)){
        line_num++;
        std::istringstream iss(line);
        std::vector<std::string> line_words;
        std::string word;
        while(iss >> word){
            line_words.push_back(word);   
        }
        this->conf.push_back({line_num, line_words});
    }

    return 0;
}

void xdr::xConfigurator::show_conf()
{
    
    for(const auto& pair : this->conf){
        // for(const auto& word : line.second()){
        //     std::cout << word << " ";
        // }
        
        
        for(size_t i = 0; i < pair.second.size(); ++i){
            std::cout << pair.first << " : " << pair.second[i] << " ";
        }
        std::cout << "\n";
    }
}

void xdr::xConfigurator::show_conf(std::vector<std::vector<std::string>> _ivec)
{
    for(const auto& line : _ivec){
        for(const auto& word : line){
            std::cout << word << " ";
        }
        std::cout << "\n";
    }    
}

std::vector<std::pair<int,std::vector<std::string>>> 
    xdr::xConfigurator::find_option(_options opt)
{
    std::vector<std::pair<int,std::vector<std::string>>> result;
    std::string opt_str = xdr::options(opt);
    for(const auto& pair : this->conf){
        for(const auto& word : pair.second){
            if(word == opt_str){
                result.push_back(pair); 
            }
        }
        // if(!result.empty()){
        //     break;
        // }
    }    
    return result;
}



int xdr::xConfigurator::save_conf()
{
    if(fs::exists(this->conf_path)){
        std::cout << XDR_PREF << "Updating xorg.conf. Removing/n";
        fs::remove(this->conf_path);
    }
    //!!!!! 
    std::ofstream output(this->conf_path);

    if(!output.is_open()){
        std::cerr << XDR_PREF << "Unable open output path/";
    } else {
        for(const auto& line : this->conf){
            for(size_t word = 0; word < line.second.size(); ++word){
                output << word << " ";
            }
            output << "\n";
        }
    }


    return 0;
}
