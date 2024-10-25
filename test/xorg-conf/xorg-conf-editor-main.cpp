#include "xorg-conf-editor.hpp"


int main(int argc, char *argv[]){
    xce app;
    app.read();
    app.cat();
    std::cout << app.search_field("Server layout") << " : " 
        << app.search_field_pos("Section \"ServerLayout\"") << std::endl;
    app.replace("FFas", 0);
    app.cat();
    app.write();
    return 0;
}