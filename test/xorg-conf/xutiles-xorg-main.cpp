#include "./xutiles-xorg.hpp"

int main(){
    xdr::textEditor test("/root/xorg.conf.new");
    test.load();
    test.show();
    //test.show_conf(test.find_option(_options::FontPath));

    return 0;
}