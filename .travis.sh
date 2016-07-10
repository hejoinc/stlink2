#!/bin/bash
if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get update -qq || true
  sudo apt-get install -qq -y --no-install-recommends libusb-1.0.0-dev
else
  brew install libusb
fi

if [[ "$CC" == "scan-build" ]]; then
    unset CC
    unset CXX
    scan-build -o analysis --status-bugs make ci
else
    make ci
fi
