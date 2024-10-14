#/bin/sh

SOURCE="src/xdr_main.cpp 
        src/xdrivers.cpp"
INCLUDE="include"

g++ $SOURCE -I$INCLUDE -lX11 -L/usr/lib/X11 -o build/xdrive-bimba