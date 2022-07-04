#!/bin/bash
g++ -o terrain_generator glslprogram.cpp diamond_square.cpp osusphere.cpp main.cpp terrain_generator.cpp -lGLEW -lGL  -lGLU  /usr/lib/x86_64-linux-gnu/libglut.so  -lm
