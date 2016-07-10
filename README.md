# stlink2

STMicroelectronics STlinkv2 programmer library and tools

# Differences

The stlink2 project is different from the already existing [OpenOCD](https://github.com/ntfreak/openocd),
 [texane/stlink](https://github.com/texane/stlink) and [blacksphere/blackmagic](https://github.com/blacksphere/blackmagic).

It is a complete clean rewrite to have full programmer control as a library with a clean defined API. As this
 can be integrated into bigger application which control multiple programmers in parallel. Written into a standalone
 C library with the only dependency to libusb. Due to libusb usage it is cross-platform and runs perfect under 
 Mac/Linux and Windows.

As it only uses the CMake build system it doesn't depend on obsecure non-portable shell-code and unix tools.

# Features

- [] Programmer-control-as-a-library
- [] Full programmer control
- [] ARM Semihosting support
- [] ARM ITM (Instrumentation macro cell) support

# License

All code is [MIT](LICENSE) licensed, except for other components under the `3thparty` folder.
