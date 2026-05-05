help: 
	@echo "Usage: make <target>"
	@echo "Targets:"
	@echo "build-aarch64: Builds the kernel for aarch64"
	@echo "build-arm: Builds the kernel for ARM 32-bit (Pi 0/1/2)"
	@echo "build-aarch64-clean: Runs cmake --fresh and builds the kernel for aarch64"
	@echo "testrp4: Tests the kernel on Qemu Raspberry Pi 4"
	@echo "testrp3: Tests the kernel on Qemu Raspberry Pi 3"
	@echo "testrp2: Tests the kernel on Qemu Raspberry Pi 2"
	@echo "testrp1: Tests the kernel on Qemu Raspberry Pi 1"
	@echo "testrp0: Tests the kernel on Qemu Raspberry Pi 0"
	@echo "get64img: Gets the aarch64 image for moving to Raspberry Pi 4/3/0 2w"
	@echo "getarm7img: Gets the arm image for moving to Raspberry pi 2,3,02w (In 32 bit mode)"
	@echo "getarmimg: Gets the arm image for moving to Raspberry pi 0w,1,cm1"


aarch64-toolchain ?= ./toolchains/aarch64.cmake

build-aarch64: 
	cmake -B build -DCMAKE_TOOLCHAIN_FILE=$(aarch64-toolchain)
	cmake --build build

build-aarch64-clean: 
	cmake --fresh -B build -DCMAKE_TOOLCHAIN_FILE=$(aarch64-toolchain)
	cmake --build build

arm-toolchain ?= ./toolchains/arm-none-eabi.cmake

build-arm:
	cmake -B build-arm -DCMAKE_TOOLCHAIN_FILE=$(arm-toolchain)
	cmake --build build-arm

testrp4:
	cmake --build build
	qemu-system-aarch64 -M raspi4b -serial stdio -kernel build/kernel8.img

testrp3:
	cmake --build build
	qemu-system-aarch64 -M raspi3b -serial stdio -kernel build/kernel8.img

# QEMU loads raw .img at 0x10000; Pi firmware loads kernel7.img at 0x8000. Use ELF here so QEMU honors the linked load address (matches real Pi).
testrp2:
	cmake --build build-arm
	qemu-system-arm -M raspi2b -m 1024 -kernel build-arm/kernel7.elf -serial stdio

testrp1:
	cmake --build build-arm
	qemu-system-arm -M raspi1ap -m 512 -kernel build-arm/kernel7.elf -serial stdio

testrp0:
	cmake --build build-arm
	qemu-system-arm -M raspi0 -m 512 -kernel build-arm/kernel7.elf -serial stdio

get64img:
	mv ./build/kernel8.img ./kernel8.img

getarm7img:
	mv ./build-arm/kernel7.img ./kernel7.img

getarmimg:
	mv ./build-arm/kernel7.img ./kernel.img


cleanbuild:
	rm -rf build
	rm -rf build-arm
	$(MAKE) build-aarch64
	$(MAKE) build-arm