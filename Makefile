CC 		:= ./toolchain/bin/i386-elf-gcc
LD 		:= ./toolchain/bin/i386-elf-ld
AS      := ./toolchain/bin/nasm

CFLAGS  := -std=c99 -pedantic -Wall -Wextra -MMD -Werror -Wno-unused-parameter
CFLAGS  += -O3 -fomit-frame-pointer
CFLAGS	+= -mno-sse -mno-mmx -mno-sse2 -mno-sse3
CFLAGS  += -Isrc/include
ASFLAGS := -felf32

all: make-all

toolchain:
	@ echo ""
	@ echo "Starting automatic cross-toolchain build. This may take a while depending on"
	@ echo "your Internet connection and processor speed."
	@ echo ""
	@ echo "The cross-toolchain will be completely contained within the toolchain/ directory. No"
	@ echo "superuser access is required."
	@ echo ""
	@ mkdir -p toolchain
	@ echo " WGET toolchain/binutils-2.24.tar.bz2"
	@ wget -P toolchain http://ftp.gnu.org/gnu/binutils/binutils-2.24.tar.bz2
	@ echo " WGET toolchain/gcc-4.8.2.tar.bz2"
	@ wget -P toolchain http://ftp.gnu.org/gnu/gcc/gcc-4.8.2/gcc-4.8.2.tar.bz2
	@ echo " WGET toolchain/nasm-2.11.02.tar.bz2"
	@ wget -P toolchain http://www.nasm.us/pub/nasm/releasebuilds/2.11.02/nasm-2.11.02.tar.xz
	@ echo " UNTAR toolchain/binutils-2.24.tar.bz2"
	@ tar -xf toolchain/binutils-2.24.tar.bz2 -C toolchain
	@ rm toolchain/binutils-2.24.tar.bz2
	@ echo " UNTAR toolchain/gcc-4.8.2.tar.bz2"
	@ tar -xf toolchain/gcc-4.8.2.tar.bz2 -C toolchain
	@ rm toolchain/gcc-4.8.2.tar.bz2
	@ echo " UNTAR toolchain/nasm-2.11.02.tar.xz"
	@ tar -xf toolchain/nasm-2.11.02.tar.xz -C toolchain
	@ rm toolchain/nasm-2.11.02.tar.xz
	@ mkdir -p toolchain/build-binutils
	@ echo ""
	@ echo " CONFIGURING BINUTILS"
	@ echo ""
	@ cd toolchain/build-binutils && ../binutils-2.24/configure \
	--target=i386-elf --prefix=$(PWD)/toolchain --disable-nls
	@ echo ""
	@ echo " COMPILING BINUTILS"
	@ echo ""
	@ make -C toolchain/build-binutils all
	@ echo ""
	@ echo " INSTALLING BINUTILS"
	@ echo ""
	@ make -C toolchain/build-binutils install
	@ echo ""
	@ echo " CLEAN toolchain/build-binutils toolchain/binutils-2.24"
	@ rm -rf toolchain/build-binutils toolchain/binutils-2.24
	@ mkdir -p toolchain/build-gcc
	@ echo ""
	@ echo " CONFIGURING GCC"
	@ echo ""
	@ export PATH=$PATH:$(PWD)/toolchain/bin
	@ cd toolchain/build-gcc && ../gcc-4.8.2/configure \
	--target=i386-elf --prefix=$(PWD)/toolchain --disable-nls \
	--enable-languages=c --without-headers
	@ echo ""
	@ echo " COMPILING GCC"
	@ echo ""
	@ make -C toolchain/build-gcc all-gcc
	@ echo ""
	@ echo " INSTALLING GCC"
	@ echo ""
	@ make -C toolchain/build-gcc install-gcc
	@ echo ""
	@ echo " CLEAN toolchain/build-gcc toolchain/gcc-4.8.2"
	@ rm -rf toolchain/build-gcc toolchain/gcc-4.8.2
	@ echo ""
	@ echo " CONFIGURING NASM"
	@ echo ""
	@ cd toolchain/nasm-2.11.02 && ./configure --prefix=$(PWD)/toolchain
	@ echo ""
	@ echo " COMPILING NASM"
	@ echo ""
	@ make -C toolchain/nasm-2.11.02
	@ echo ""
	@ echo " INSTALLING NASM"
	@ echo ""
	@ make -C toolchain/nasm-2.11.02 install
	@ echo ""
	@ echo " CLEAN toolchain/nasm-2.11.02"
	@ rm -rf toolchain/nasm-2.11.02
	
make-all
	@echo "Make all"