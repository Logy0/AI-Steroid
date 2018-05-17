#!/bin/sh
cd cmake

if [ $# -eq 1 ]; then
	cmake . -DCMAKE_BUILD_TYPE=Debug
else
	cmake . -DCMAKE_BUILD_TYPE=Release
fi
	
make

cd ../../bin
chmod u+x *
 
if [ $# -eq 1 ]; then
	cd Debug
	./AI-Steroid.exe
else
	cd Release
	./AI-Steroid.exe
fi
