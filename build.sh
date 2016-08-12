#!/bin/bash

autoscan
aclocal
autoconf
automake --add-missing

mkdir build
cd build
../configure && make
