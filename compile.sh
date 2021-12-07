#!/bin/bash
g++ -o final glslprogram.cpp diamond_square.cpp osusphere.cpp main.cpp mckeever_final.cpp -lGLEW -lGL  -lGLU  /usr/lib/x86_64-linux-gnu/libglut.so  -lm

g++ -o test1 test.cpp diamond_square.cpp -lGLEW -lGL  -lGLU  /usr/lib/x86_64-linux-gnu/libglut.so  -lm
