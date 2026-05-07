default: cleanbuild

.PHONY: default help cleanbuild test-aarch64 test-arm install

help: 
	@echo "Usage: make <target>"
	@echo "Targets:"
	@echo "cleanbuild: Removes the build directories and rebuilds the kernel (default target)"
	@echo "--- Individual build modes removed since they wouldn't do a full build ---"
	@echo "test-aarch64: Tests the 64 bit kernel (Qemu rpi4b)"
	@echo "test-arm: Tests the 32 bit kernel  (Qemu rpi0)"
	@echo "install: Runs cleanbuild then copies kernel8.img, kernel7.img, and kernel.img to a directory"
	@echo "         Usage: make install DEST=/path/to/dir   (or just 'make install' to be prompted)"

aarch64-toolchain ?= ./toolchains/aarch64.cmake
arm-toolchain ?= ./toolchains/arm-none-eabi.cmake


cleanbuild:
	rm -rf build
	rm -rf build-arm
	cmake -B build -DCMAKE_TOOLCHAIN_FILE=$(aarch64-toolchain)
	cmake --build build
	cmake -B build-arm -DCMAKE_TOOLCHAIN_FILE=$(arm-toolchain)
	cmake --build build-arm
	cp build-arm/kernel7.img build-arm/kernel.img

test-aarch64:
	$(MAKE) cleanbuild
	qemu-system-aarch64 -M raspi4b -serial stdio -kernel build/kernel8.img

test-arm:
	$(MAKE) cleanbuild
	qemu-system-arm -M raspi0 -m 512 -kernel build-arm/kernel7.elf -serial stdio

install: cleanbuild
ifdef DEST
	@mkdir -p "$(DEST)"
	cp build/kernel8.img "$(DEST)/"
	cp build-arm/kernel7.img "$(DEST)/"
	cp build-arm/kernel.img "$(DEST)/"
	@echo "Installed kernels to $(DEST)"
else
	@read -p "Enter destination directory: " dest && \
	    test -n "$$dest" && \
	    mkdir -p "$$dest" && \
	    cp build/kernel8.img "$$dest/" && \
	    cp build-arm/kernel7.img "$$dest/" && \
	    cp build-arm/kernel.img "$$dest/" && \
	    echo "Installed kernels to $$dest"
endif

