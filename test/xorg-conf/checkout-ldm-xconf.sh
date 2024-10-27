#!/bin/bash

WIDNOW_MANAGER=gdm.service
XPRF="[XCONF] "

(sleep 10 && $XPRF"Timer expired.") & timer_pid=$!;

if systemctl is-active --quiet gdm.service; then
    echo $XPRF"Service started correctly!"
    kill $timer_pid
else
    # condition sucks 
    wait $timer_pid 
    # postscript if not works 
    mv /etc/X11/xorg.conf /etc/X11/xorg.conf.notbooted
if
