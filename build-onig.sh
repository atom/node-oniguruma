cd vendor/onig
./configure
make
cp .libs/libonig.a ../../src/libonig.a
cp oniguruma.h ../../src/oniguruma.h
