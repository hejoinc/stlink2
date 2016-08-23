#!/bin/bash
if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get update -qq || true
  sudo apt-get install -qq -y --no-install-recommends gdb libusb-1.0.0-dev
else
  brew install libusb
fi

if [[ "$CC" == "scan-build" ]]; then
    unset CC
    unset CXX
    scan-build -o analysis --status-bugs make ci
else
    lsusb
    make debug
    gdb -batch -ex "run" -ex "bt" build/Debug/tests/open_close 2>&1 | grep -v ^"No stack."$
    make ci
fi
