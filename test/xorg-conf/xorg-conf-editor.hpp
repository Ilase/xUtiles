#ifndef XCE_HPP
#define XCE_HPP

#define DEBUG

#include <vector>
#include <utility>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>

const std::vector<std::pair<std::string, std::string>> tag_dict = {
    {"Section","SectionEnd"},
    {"SubSection","SubSectionEnd"}
};

namespace xdr{
    class xConf{
        std::filesystem::path xorg_conf = "/etc/X11/xorg.conf";
        public:
        xConf();
        int generate_config();
        int change_param();
    };

}




class xce{
    std::filesystem::path conf_path = ("xorg.conf");
    std::vector<std::string> file_body;
    public:
    //xce();
    std::string search_field(std::string _sr){
        for(const auto& str : this->file_body){
            if(str.find(_sr) != str.npos){ //no matches find in string
                return str;
            }
        }
        return "Err";
    };
    int search_field_pos(std::string _sr){
        auto it = std::find(this->file_body.begin(), this->file_body.end(),_sr);
        if(it != this->file_body.end()){
            return std::distance(this->file_body.begin(), it);
        }
        return -1;
    };
    int read(){
        std::ifstream conf = this->conf_path;
        if(!conf){
            std::cerr << "Can't read file data:" << conf_path.string() << std::endl;
            return 1;
        }
        std::string temp;
        for(size_t i = 0; getline(conf, temp); ++i){
            this->file_body.push_back(temp); 
        }
        return 0;
    };
    int cat(){
        for(auto target : this->file_body){
            std::cout << target << std::endl;
        }
        return 0;
    };
    int replace(std::string _f, std::string _r){
        std::cout << _f << std::endl;
        for(size_t i = 0; i < this->file_body.size(); ++i){
#ifdef DEBUG
            std::cout << "Matching with: " << file_body[i] << std::endl;
#endif 
            if(_f == file_body[i]){
                file_body[i] = _r;
                return 0;
            } else{
                return -1;
            }
        }
        return 0;
    };
    int replace(std::string _f, int _ind){
        this->file_body[_ind] = _f;
        return 0;
    };
    int write(){
        std::filesystem::path source = "xorg.conf";
        std::filesystem::path dest = "xorg.conf.old";

        try{
            std::filesystem::rename(source, dest);
            std::cout << "Saving complete succsessfuly." << std::endl;
        }catch(const std::filesystem::filesystem_error &e){
            std::cerr << "Err: " << e.what() << std::endl;
        }

        std::ofstream out("xorg.conf");
        if(!out){
            std::cerr << "Can't open file for writing." << std::endl;
            return 1;
        }
        for(const auto& line : this->file_body){
            out << line << std::endl;
        }
        out.close();
        if(!out.good()){
            std::cerr << "Error occured while writing to file." << std::endl;
        }
        return 0;
    };

};

#endif