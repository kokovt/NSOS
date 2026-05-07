#include "peripherals/base.h"

uint32_t get_PBASE(void) {
  uint32_t part;

#if defined(__aarch64__)
  uint64_t reg;
  asm volatile("mrs %0, midr_el1" : "=r"(reg));
  part = (uint32_t)((reg >> 4) & 0xFFF);
#elif defined(__arm__)
  uint32_t reg;
  asm volatile("mrc p15,0,%0,c0,c0,0" : "=r"(reg));
  part = (reg >> 4) & 0xFFF;
#else
#error Unsupported architecture for get_PBASE
#endif
  switch (part) {
  case 0xB76:
    return 0x20000000; // ARM1176 (Pi 0/1)
  case 0xC07:
  case 0xD03:
    return 0x3F000000; // Cortex-A53 (Pi 3)
  case 0xD07:
  case 0xD08:
    return 0xFE000000; // Cortex-A72 (Real Pi 4 CPU)
  default:
    return 0x20000000;
  }
}