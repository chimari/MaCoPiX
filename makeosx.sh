#!/bin/sh
touch configure configure.ac aclocal.m4 Makefile.am Makefile.in

./configure CPPFLAGS=-I/usr/local/opt/openssl/include \
  && make
