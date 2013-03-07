cd vendor/onig
CPPFLAGS="$1"
CFLAGS="$1"
./configure
make clean
make
cp .libs/libonig.a ../../src/libonig.a
cp oniguruma.h ../../src/oniguruma.h
