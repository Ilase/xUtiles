#include "./xutiles-xorg.hpp"

int main(){
    xdr::xConfigurator test("/root/xorg.conf.new");
    test.read_conf();
    test.show_conf();
    //test.show_conf(test.find_option(_options::FontPath));

    return 0;
}