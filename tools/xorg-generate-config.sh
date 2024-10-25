#!/bin/bash

main(){
    Xorg :2 -configure
    cp /root/xorg.conf.new /etc/X11/xorg.conf
}

set +e
main