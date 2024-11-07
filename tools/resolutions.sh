#!/bin/bash

CVT="$(cvt $1 $2 $3)"

echo $CVT | sed 's/Modeline/\nxrandr --newmode/' >> ~/.xprofile 

