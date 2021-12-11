#!/bin/bash
echo "build matrice"
gcc matrice.c -o matrice `pkg-config --cflags --libs glib-2.0` -O2 -g -Wall -Wextra -Wno-unused-result  -Wno-unused-parameter
