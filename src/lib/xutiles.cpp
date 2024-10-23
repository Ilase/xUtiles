#include "xutiles.hpp"

xdr::xUtiles::xUtiles(int _argc, char *_argv[]) : argc(_argc)
{
    this->args = std::vector<std::string>(_argv + 1, _argv + _argc);
}

std::string xdr::exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get()))
    {
        if (fgets(buffer, 128, pipe.get()) != NULL){
            result += buffer;
        }
    }
    return result;
}

int xdr::xUtiles::app()
{
    if(argc < 2){
        std::cerr << XDR_PREF << "No comand provided" << std::endl;
        return XDR_ERR;
    }
    auto &largs = this->args;
    xdr::xBackup app;
    // Call constructor of main class
    //
    // link to public field!!!
    //
    //this thing make argv more easy but idont know how it works
    for(size_t i = 0; i < this->args.size() ; ++i){
        const auto& arg = this->args[i];
        auto it = comand_map.find(arg);
        if (it !=comand_map.end()){ 
            std::visit([&app, &largs, &i](auto&& func){
                using func_type = std::decay_t<decltype(func)>;
                if constexpr (std::is_invocable_v<func_type, xdr::xBackup&>){
                    func(app);   
                } else if constexpr (std::is_invocable_v<func_type, xdr::xBackup&, const std::string&>){
                    if(i + 1 < largs.size()){
                        func(app, largs[i + 1]);
                        ++i;
                    } else {
                        std::cerr << XDR_PREF << "Error: Missing path arg: " << largs[i] << std::endl; 
                    }
                }else {
                    func();
                }
                //switch to second cell in map with lambda function if 
                // it have right condition
            }, it->second);
        }else{
            std::cout << XDR_PREF << "Unknown argument: " << arg << std::endl; 
        }
    } 
    

    return XDR_OK;
}
