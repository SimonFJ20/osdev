#!/bin/sh

echo "GNU GCC and GNU Binutils will be installed into in $HOME/osdev/cross/"
echo "This script should be run from inside the $HOME/osdev/ folder"
echo "Make is set to use 16 CPU threads, if it shouldn't, change it"
echo ""
read -p "Do you want to continue? " -n 1 -r
echo    # (optional) move to a new line
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    [[ "$0" = "$BASH_SOURCE" ]] && exit 1 || return 1 # handle exits from shell or function but don't exit interactive shell
fi



mkdir -p $HOME/osdev/src
cd $HOME/osdev/src

read -p "Should new sourcecode be downloaded? " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    wget "https://ftp.gnu.org/gnu/binutils/binutils-2.38.tar.gz"
    wget "https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz"
    tar -xf "binutils-2.38.tar.gz"
    tar -xf "gcc-11.2.0.tar.gz"
fi

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


