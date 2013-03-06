cd vendor/onig
if [ "$npm_config_arch" == "x86_32" ]; then
  CPPFLAGS=-m32
  CFLAGS=-m32
fi
./configure
make clean
make
cp .libs/libonig.a ../../src/libonig.a
cp oniguruma.h ../../src/oniguruma.h
