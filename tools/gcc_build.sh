#/bin/sh

SOURCE="src/lib/xutiles-backup.cpp 
        src/lib/xutiles-drivers.cpp
        src/lib/xutiles-main-bin.cpp
        src/lib/xutiles.cpp"
INCLUDE="include/lib/"

g++ $SOURCE -I$INCLUDE -lX11 -L/usr/lib/X11 -o build/xdrive-bimba