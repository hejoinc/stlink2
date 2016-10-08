# stlink2

STMicroelectronics STlinkv2 programmer library and tools

[![license](https://img.shields.io/github/license/xor-gate/stlink2.svg?maxAge=2592000)]()
[![Build Status](https://img.shields.io/travis/xor-gate/stlink2/master.svg?label=Mac%20OSX%20%26%20Linux)](https://travis-ci.org/xor-gate/stlink2)
[![Build status](https://img.shields.io/appveyor/ci/xor-gate/stlink2/master.svg?label=Windows)](https://ci.appveyor.com/project/xor-gate/stlink2)

# Differences

The stlink2 project is different from the already existing [OpenOCD](https://github.com/ntfreak/openocd),
 [texane/stlink](https://github.com/texane/stlink) [pavelrevak/pystlink](https://github.com/pavelrevak/pystlink), [avrhack/stlink-trace](https://github.com/avrhack/stlink-trace), [fpoussin/QStlink2](https://github.com/fpoussin/QStlink2) and [blacksphere/blackmagic](https://github.com/blacksphere/blackmagic).

It is a complete clean rewrite to have full programmer control as a library with a clean defined API. As this
 can be integrated into bigger application which control multiple programmers in parallel. Written into a standalone
 C library with the only dependency to libusb. Due to libusb usage it is cross-platform and runs perfect under 
 Mac/Linux and Windows.

As it only uses the CMake build system it doesn't depend on obsecure non-portable shell-code and unix tools.

# Building

* C99 compatible compiler (gcc, clang, msvc)
* CMake 2.8 or higher
* libusb 1.0.9 or higher

```
make debug
make release
make test
```

# Tested

The software is tested from time-to-time on the following platforms:

* Mac OS X 10.11 AMD64
* Debian 8.0 AMD64
* OpenBSD 6.0 AMD64
* FreeBSD 11-RC2 AMD64

# License

All code is [MIT](LICENSE) licensed, except for other components under the `3thparty` folder.
