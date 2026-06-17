cmake_minimum_required(VERSION 3.15)
project(NSOS C ASM)

# The build toolchains for aarch64 and arm are close enough to be combined with a single if condition
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -nostdlib -O2 -ffreestanding -mgeneral-regs-only")
  set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(KERNEL_NAME "kernel8")
  else()
      set(KERNEL_NAME "kernel7")
  endif()

  file(GLOB_RECURSE C_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
  file(GLOB_RECURSE ASM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.S")

  add_executable(${KERNEL_NAME}.elf ${C_FILES} ${ASM_FILES})
  target_include_directories(${KERNEL_NAME}.elf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kernel")

  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld")
  else()
      set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/linker-arm.ld")
  endif()
  target_link_options(${KERNEL_NAME}.elf PRIVATE
      "-T" "${LINKER_SCRIPT}"
      "-nostdlib"
      "-Wl,-I${CMAKE_CURRENT_SOURCE_DIR}/include/kernel"
      "-Wl,-Map=${KERNEL_NAME}.map"
  )

  target_link_libraries(${KERNEL_NAME}.elf PRIVATE gcc)

  add_custom_command(
      TARGET ${KERNEL_NAME}.elf POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${KERNEL_NAME}.elf> -O binary ${KERNEL_NAME}.img
      COMMENT "Generating ${KERNEL_NAME}.img from ${KERNEL_NAME}.elf"
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64") 
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -D_USE_LIMINE_")
  set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mno-red-zone -D_USE_LIMINE_")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  file(GLOB_RECURSE C_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
  file(GLOB_RECURSE ASM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.S")

  add_executable(NSOS.elf ${C_FILES} ${ASM_FILES})
  target_include_directories(NSOS.elf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kernel")

  target_link_options(NSOS.elf PRIVATE 
    "-T" "${CMAKE_CURRENT_SOURCE_DIR}/src/linker-x86_64.ld"
    "-nostdlib"
    "-Wl,-I${CMAKE_CURRENT_SOURCE_DIR}/include/kernel"
    "-Wl,-Map=NSOS.elf.map")

endif()
