#/bin/sh

SOURCE="src/xutiles-backup.cpp 
        src/xutiles-drivers.cpp
        src/xutiles-main-bin.cpp
        src/xutiles.cpp"
INCLUDE="include"

g++ $SOURCE -I$INCLUDE -lX11 -L/usr/lib/X11 -o build/xdrive-bimba