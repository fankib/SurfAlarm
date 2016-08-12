#!/bin/bash

autoscan
autoconf
automake

mkdir build
cd build
../configure && make
