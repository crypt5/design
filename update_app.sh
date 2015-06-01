#!/bin/bash

ntpdate -b -s -u pool.ntp.org

git pull
rm includes/*.h
rm libs/*.so

cd ../Utilities
git pull
make -B

cd ..
cp Utilities/Output/*.h design/includes
cp Utilities/Output/BBBio/*.h design/includes
cp Utilities/Output/Graphics/*.h design/includes

cp Utilities/Output/*.so design/libs
cp Utilities/Output/BBBio/*.so design/libs
cp Utilities/Output/Graphics/*.so design/libs

cd design
make -B

read -p "Press [ENTER] to exit"
