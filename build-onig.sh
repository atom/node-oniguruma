#!/bin/sh

cd deps/onig
if [ ! -f "Makefile" ]; then
  CFLAGS="$1" ./configure --disable-shared
fi
make
