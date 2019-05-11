#!/bin/sh
touch configure configure.ac aclocal.m4 Makefile.am Makefile.in

./configure --with-win32 --with-gtk3\
  'CC=gcc' CFLAGS=-O2 \
  && make \
  && make install-strip prefix=/home/taji/dist64
