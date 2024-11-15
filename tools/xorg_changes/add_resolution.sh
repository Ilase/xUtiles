#!/bin/bash

CONF='/etc/X11/xorg.conf.d/10-resolution.conf'

MODELINE="$(cvt $2 $3 $4 | grep Modeline)"
if [ ! -f $CONF ]; 
then
    mkdir -p /etc/X11/xorg.conf.d
    touch $CONF
    cat <<EOF > $CONF
Section "Monitor"
    Identifier "$1"
EndSection
EOF
fi

if grep -q $MODELINE $CONF; then
    return 1
fi

if ! grep -q "Identifier \"$1\"" $CONF; then
    cat <<EOF >> $CONF
Section "Monitor"
    Identifier "$1"
EndSection
EOF
fi

sed -i -e "s/Identifier \"$1\"/Identifier \"$1\"\n    $MODELINE/" $CONF
#sed -i -e "/Identifier $1/a $MODELINE" $CONF
