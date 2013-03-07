cd vendor/onig
CPPFLAGS="$1"
CFLAGS="$1"
./configure --disable-shared
make clean
make
cp .libs/libonig.a ../../src/libonig.a
cp oniguruma.h ../../src/oniguruma.h
rm -fr .libs
