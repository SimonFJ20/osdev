#!/bin/sh

mkdir -p $HOME/osdev/src
cd $HOME/osdev/src
#wget "https://ftp.gnu.org/gnu/binutils/binutils-2.38.tar.gz" 
#wget "https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz"

export PREFIX="$HOME/osdev/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p build-binutils
cd build-binutils
../binutils-2.38/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j16
make install

cd $HOME/osdev/src
 
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-gcc
cd build-gcc
../gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j16
make all-target-libgcc -j16
make install-gcc
make install-target-libgcc


