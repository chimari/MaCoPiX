#!/bin/sh

./configure --with-win32 --with-tar32\
  'CC=gcc -mtune=pentium3' CFLAGS=-O3 \
  && make \
  && make install-strip prefix=$HOME/dist
