# Define variables globally at the top so they persist
SHELL := /bin/bash

PREFIX=$(HOME)/opt/cross
THREADS=8

help:
	@echo "Usage: make <target>"
	@echo "Note: This script only works on Linux. It will not work on MacOS or Windows."
	@echo "This script requires the binutils-2.46.0.tar.xz and gcc-15.2.0.tar.xz files to be in the current directory."
	@echo "Make sure to have gcc-15 downloaded, and to run CC=gcc-15 and CXX=g++-15 before running this script."
	@echo "https://ftp.gnu.org/gnu/binutils/binutils-2.46.0.tar.xz"
	@echo "https://ftp.gnu.org/gnu/gcc/gcc-15.2.0/gcc-15.2.0.tar.xz"
	@echo ""
	@echo "Targets:"
	@echo "build-aarch64: Builds the toolchain for Arm 64-Bit (For RPI4, RPI3)"
	@echo "build-arm-none-eabi: Builds the toolchain for Arm 32-Bit (For RPI0, RPI1, RPI2, RPI3)"

define BUILD_TOOLCHAIN
	@echo "Cleaning up old build files for $(1)"
	rm -rf build-binutils build-gcc binutils-2.46.0 gcc-15.2.0

	@echo "Extracting binutils-2.46.0.tar.xz"
	tar -xf binutils-2.46.0.tar.xz
	mkdir build-binutils

	@echo "Configuring, building, and installing binutils"
	cd build-binutils && \
		../binutils-2.46.0/configure \
			--target=$(1) \
			--prefix="$(PREFIX)" \
			--with-sysroot \
			--disable-nls \
			--disable-werror && \
		make -j$(THREADS) && \
		make install

	@echo "Extracting gcc-15.2.0.tar.xz"
	tar -xf gcc-15.2.0.tar.xz
	mkdir build-gcc

	@echo "Checking for $(1)-as"
	@if [ ! -f "$(PREFIX)/bin/$(1)-as" ]; then \
		echo "Error: $(1)-as not found in $(PREFIX)/bin/, manual intervention required."; \
		exit 1; \
	fi

	@echo "Configuring and building GCC for $(1)"
	cd build-gcc && \
		../gcc-15.2.0/configure --target=$(1) --prefix="$(PREFIX)" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx && \
		make all-gcc -j$(THREADS) && \
		make all-target-libgcc -j$(THREADS) && \
		make all-target-libstdc++-v3 -j$(THREADS) && \
		make install-gcc && \
		make install-target-libgcc && \
		make install-target-libstdc++-v3

	@echo "Build successful for $(1)!"
	@echo "Remember to add $(PREFIX)/bin to your PATH!"
endef

build-aarch64: binutils-2.46.0.tar.xz gcc-15.2.0.tar.xz
	$(call BUILD_TOOLCHAIN,aarch64-elf)


build-arm-none-eabi: binutils-2.46.0.tar.xz gcc-15.2.0.tar.xz
	$(call BUILD_TOOLCHAIN,arm-none-eabi)

build-x86_64-elf: binutils-2.46.0.tar.xz gcc-15.2.0.tar.xz 
	$(call BUILD_TOOLCHAIN,x86_64-elf)
