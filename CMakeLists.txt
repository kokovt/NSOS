cmake_minimum_required(VERSION 3.15)
project(NSOS C ASM)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -nostdlib -O2 -ffreestanding -mgeneral-regs-only")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Determine the Kernel Name based on architecture
# If your toolchain sets CMAKE_SYSTEM_PROCESSOR to 'arm' or 'aarch64'
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(KERNEL_NAME "kernel8")
else()
    set(KERNEL_NAME "kernel7")
endif()

file(GLOB_RECURSE C_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE ASM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.S")

# 2. Use the variable for the executable name
add_executable(${KERNEL_NAME}.elf ${C_FILES} ${ASM_FILES})
target_include_directories(${KERNEL_NAME}.elf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/ld/linker.ld")
else()
    set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/ld/linker-arm.ld")
endif()
target_link_options(${KERNEL_NAME}.elf PRIVATE 
    "-T" "${LINKER_SCRIPT}" 
    "-nostdlib" 
    "-Wl,-I${CMAKE_CURRENT_SOURCE_DIR}/include"
    "-Wl,-Map=${KERNEL_NAME}.map"
)

# 3. Reference the target and variable in the post-build command
add_custom_command(
    TARGET ${KERNEL_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${KERNEL_NAME}.elf> -O binary ${KERNEL_NAME}.img
    COMMENT "Generating ${KERNEL_NAME}.img from ${KERNEL_NAME}.elf"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)