#!/bin/bash
set +e 
# -----------------------------------
# <cmdline>
# /etc/X11/<cmdline>
# /usr/etc/X11/<cmdline>
# $XORGCONFIG
# /etc/X11/$XORGCONFIG
# /usr/etc/X11/$XORGCONFIG
# /etc/X11/xorg.conf
# /etc/xorg.conf
# /usr/etc/X11/xorg.conf.<hostname>
# /usr/etc/X11/xorg.conf
# /usr/lib/X11/xorg.conf.<hostname>
# /usr/lib/X11/xorg.conf
# -----------------------------------
# Important shortcut??
# /etc/init.d/gdm stop || /etc/init.d/gdm3 stop || /etc/init.d/kdm stop || /etc/init.d/xdm stop || /etc/init.d/lightdm stop

function main(){
    echo "Xorg conf builder!";
    cd /etc/X11/
    systemctl stop lightdm
    Xorg -configure
    systemctl enable --now lightdm
}


main &