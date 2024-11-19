#!/bin/bash

set -e 

BASE_NAME=$(basename *.pro .pro)

if [ ! -d "build" ]; then
    mkdir build
    mv -r ../cache build
fi
cd build


qmake ../*.pro
make clean
make
./"$BASE_NAME"
