#!/bin/sh
touch configure configure.ac aclocal.m4 Makefile.am Makefile.in

./configure --with-win32 --with-tar64\
  'CC=gcc' CFLAGS=-O2 \
  && make \
  && make install-strip prefix=/home/${USER}/dist64
