#include "xdrivers.hpp"


int main(int argc, char const *argv[])
{   

    fs::path backup_path = "/mnt/xdr-backups/";
    fs::path X11_d = "/etc/X11/";
    fs::path MDP_d = "/etc/modprobe.d/";
    //auto app = xdr::xDriver(); error out double 
    xdr::make_backup(backup_path, X11_d,MDP_d);
    return 0;
}
