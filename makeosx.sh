#!/bin/sh
touch configure configure.ac aclocal.m4 Makefile.am Makefile.in

## libffi & openssl installed as keg-only in Homebrew
./configure --with-gtkmac PKG_CONFIG_PATH="/usr/local/opt/libffi/lib/pkgconfig:/usr/local/opt/openssl/lib/pkgconfig" \
  && make
