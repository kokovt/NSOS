# Repo root = directory of this Makefile (works even if make's cwd is elsewhere)
ROOT := $(shell cd "$(dir $(lastword $(MAKEFILE_LIST)))" && pwd)

aarch64-toolchain ?= $(ROOT)/toolchains/aarch64.cmake
arm-toolchain ?= $(ROOT)/toolchains/arm-none-eabi.cmake
x86_64-toolchain ?= $(ROOT)/toolchains/x86_64.cmake

NPROC := $(shell nproc 2>/dev/null || echo 4)
# Verbose compile/link lines: make V=1 cleanbuild
BUILD_VERBOSE := $(if $(filter 1,$(V)),--verbose,)

BUILD_AARCH64 := $(ROOT)/build
BUILD_ARM := $(ROOT)/build-arm
BUILD-X86_64 := ${ROOT}/build-x86_64

default: cleanbuild

.PHONY: default help cleanbuild test-aarch64 test-arm install

help:
	@echo "Usage: make <target>"
	@echo "Targets:"
	@echo "cleanbuild: Removes the build directories and rebuilds the kernel (default target)"
	@echo "            Optional: V=1 verbose, JOBS=N parallel jobs (defaults to nproc)"
	@echo "--- Individual build modes removed since they wouldn't do a full build ---"
	@echo "test-aarch64: Tests the 64 bit kernel (QEMU rpi4b)"
	@echo "test-arm: Tests the 32 bit kernel  (QEMU rpi0)"
	@echo "install: Runs cleanbuild then copies kernel8.img, kernel7.img, and kernel.img to a directory"
	@echo "         Usage: make install DEST=/path/to/dir   (or just 'make install' to be prompted)"

setuplimine:
	curl -L https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz | gunzip | tar -xf -
	make -C limine-binary

cleanbuild:
	rm -rf "$(BUILD_AARCH64)" "$(BUILD_ARM)" "${BUILD-X86_64}"
	cmake -S "$(ROOT)" -B "$(BUILD_AARCH64)" -DCMAKE_TOOLCHAIN_FILE="$(aarch64-toolchain)"
	cmake --build "$(BUILD_AARCH64)" -j$(or $(JOBS),$(NPROC)) $(BUILD_VERBOSE)
	cmake -S "$(ROOT)" -B "$(BUILD_ARM)" -DCMAKE_TOOLCHAIN_FILE="$(arm-toolchain)"
	cmake --build "$(BUILD_ARM)" -j$(or $(JOBS),$(NPROC)) $(BUILD_VERBOSE)
	cp "$(BUILD_ARM)/kernel7.img" "$(BUILD_ARM)/kernel.img"
	cmake -S "${ROOT}" -B "$(BUILD-X86_64)" -DCMAKE_TOOLCHAIN_FILE="$(x86_64-toolchain)"
	cmake --build "$(BUILD-X86_64)" -j$(or $(JOBS),$(NPROC)) $(BUILD_VERBOSE)
	mkdir -p ./build-x86_64/iso_root/boot/limine/
	mkdir -p ./build-x86_64/iso_root/EFI/BOOT/
	curl -o ./build-x86_64/limine-binary.tar.gz -L https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz
	mkdir ./build-x86_64/limine-binary
	tar -xf ./build-x86_64/limine-binary.tar.gz -C ./build-x86_64/limine-binary --strip-components=1
	cd ./build-x86_64/limine-binary/ && make
	cp -v ./src/limine.conf ./build-x86_64/limine-binary/limine-bios.sys ./build-x86_64/limine-binary/limine-bios-cd.bin ./build-x86_64/limine-binary/limine-uefi-cd.bin ./build-x86_64/iso_root/boot/limine
	cp -v ./build-x86_64/limine-binary/BOOTX64.EFI ./build-x86_64/limine-binary/BOOTIA32.EFI ./build-x86_64/iso_root/EFI/BOOT/
	cp -v ./build-x86_64/NSOS.bin ./build-x86_64/iso_root/boot/
	xorriso -as mkisofs -R -r -J -b /boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot /boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        build-x86_64/iso_root/ -o image.iso


test-aarch64: cleanbuild
	qemu-system-aarch64 -M raspi4b -serial stdio -kernel "$(BUILD_AARCH64)/kernel8.img"

test-arm: cleanbuild
	qemu-system-arm -M raspi0 -m 512 -kernel "$(BUILD_ARM)/kernel7.elf" -serial stdio

install: cleanbuild
ifdef DEST
	@mkdir -p "$(DEST)"
	cp "$(BUILD_AARCH64)/kernel8.img" "$(DEST)/"
	cp "$(BUILD_ARM)/kernel7.img" "$(DEST)/"
	cp "$(BUILD_ARM)/kernel.img" "$(DEST)/"
	@echo "Installed kernels to $(DEST)"
else
	@read -p "Enter destination directory: " dest && \
	    test -n "$$dest" && \
	    mkdir -p "$$dest" && \
	    cp "$(BUILD_AARCH64)/kernel8.img" "$$dest/" && \
	    cp "$(BUILD_ARM)/kernel7.img" "$$dest/" && \
	    cp "$(BUILD_ARM)/kernel.img" "$$dest/" && \
	    echo "Installed kernels to $$dest"
endif
