#!/bin/bash

WINDOW_MANAGER=lightdm.service
XPRF="[XCONF] "

(sleep 10 && echo $XPRF"Timer expired.") &
timer_pid=$!

while kill -0 $timer_pid 2>/dev/null; do
    systemctl restart lightdm.service
    if systemctl is-active --quiet $WINDOW_MANAGER; then
        echo $XPRF"Service started correctly!"
        kill $timer_pid
        break
    fi
    sleep 1
done

wait $timer_pid
if ! systemctl is-active --quiet $WINDOW_MANAGER; then
    mv /etc/X11/xorg.conf /etc/X11/xorg.conf.notbooted
    mv /etc/X11/xorg.conf.d /etc/X11/xorg.conf.d.notbooted
fi