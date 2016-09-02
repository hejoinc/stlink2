# stlink2

STMicroelectronics STlinkv2 programmer library and tools

[![Build Status](https://travis-ci.org/xor-gate/stlink2.svg?branch=master)](https://travis-ci.org/xor-gate/stlink2)
[![Build status](https://ci.appveyor.com/api/projects/status/u36ning58lxy7kwh?svg=true)](https://ci.appveyor.com/project/xor-gate/stlink2)

# Differences

The stlink2 project is different from the already existing [OpenOCD](https://github.com/ntfreak/openocd),
 [texane/stlink](https://github.com/texane/stlink) [pavelrevak/pystlink](https://github.com/pavelrevak/pystlink), [avrhack/stlink-trace](https://github.com/avrhack/stlink-trace), [fpoussin/QStlink2](https://github.com/fpoussin/QStlink2) and [blacksphere/blackmagic](https://github.com/blacksphere/blackmagic).

It is a complete clean rewrite to have full programmer control as a library with a clean defined API. As this
 can be integrated into bigger application which control multiple programmers in parallel. Written into a standalone
 C library with the only dependency to libusb. Due to libusb usage it is cross-platform and runs perfect under 
 Mac/Linux and Windows.

As it only uses the CMake build system it doesn't depend on obsecure non-portable shell-code and unix tools.

# License

All code is [MIT](LICENSE) licensed, except for other components under the `3thparty` folder.
