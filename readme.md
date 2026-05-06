# NSOS

This is a very early WIP operating system designed to be used on a plethora of Raspberry PI boards.


## Features

[Currently Broken] UART Outputting (Only tested on Qemu)

Creating a framebuffer

Drawing to the screen (Circles, rectangles, a basic font, lines, and individual pixel placement)

## Supported boards

<details>
<summary>
    <b>[Expand]</b> The code should support PI0[2], PI1,2,3,4<br/>The code has been tested on a RPI 400, and a RPI 0w
</summary>


Some boards *are theoretically supported, by testing on qemu, but might xt actually run*.<br/>
An example of this happening is on my PI 400, where it booted and displayed on the qemu 4b emulation, but didn't run on the actual board.

Over time, I will try to Reduce the number of boards that are supported 


| Board   | Supported | Tested on board | Tested on Qemu | Physical Board Variants Tested |
| ------- | --------- | --------------- | -------------- | ------------------------------ |
| PI5     | x         | x               | x              | x                              |
| PI4     | ✓        |   ✓             | ✓              | RP400                          |
| PI3     | ✓        | x               | ✓              | x                              |
| PI2     | ✓        | x               | ✓              | x                              |
| PI1     | ✓        | x               | ✓              | x                              |
| PI02W   | ✓        | x               | x              | x                              |
| PI0     | ✓        | ✓               | ✓              | RP0W                           |
| PI PICO | x        | x               | x              | x                              |
| CM      | x        | x               | x              | x                              |
</details>

## Supported Architectures
<details>

<summary>
    The code suports AARCH64, and ARM32 on the Raspberry PI's bootloader.<br/>
    The code is planned to support x86-64 through the Limine bootloader in the future.
</summary>

This is a list of architectures I plan on making it support; and a list of what is supports 

* AARCH64 (RISCV) [ARM64] 
* ARM32   (RISCV)
* x86-64  (CISC)
* x86     (CISC)


| Architecture | Bootloader            | Tested on QEMU | Tested on Physical CPU | Currently Supported |
| ------------ | --------------------- | -------------- | ---------------------- | ------------------- |
| AARCH64      | Raspberian Bootloader | ✓            | PI 400                 | ✓                 |
| ARM32        | Raspberian Bootloader | ✓            | PI 0W                  | ✓                 |
| x86-64       | Limine                | x             | x                     | x                  |
| x86          | Limine                | x             | x                     | x*                 |

\* I don't have any x86 computers

</details>