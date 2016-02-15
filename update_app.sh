#!/bin/bash

ntpdate -b -s -u pool.ntp.org

cd ../Utilities
git pull
make -B install clean

cd ../Graphics
git pull
make -B install clean

cd ../design
git pull
make -B all clean

read -p "Press [ENTER] to exit"
