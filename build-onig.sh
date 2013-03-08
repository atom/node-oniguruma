cd deps/onig
CFLAGS="$1" ./configure --disable-shared
make clean
make
