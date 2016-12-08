#!/bin/sh

aclocal  \
  && libtoolize --force --copy \
  && autoheader \
  && automake --add-missing --foreign --copy \
  && autoconf \
  && ./configure $@
