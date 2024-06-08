#!/usr/bin/env bash

set -e

cd "$(dirname "${BASH_SOURCE[0]}")"

THREAD_NUM=$(nproc)

# https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
# Install gtest.
VERSION="1.12.1"
PKG_NAME="googletest-release-${VERSION}.tar.gz"

tar xzf "${PKG_NAME}"
pushd googletest-release-${VERSION}
    mkdir build && cd build
    cmake ..
    make -j${THREAD_NUM}
    make install
popd

ldconfig

# cleanup
rm -rf PKG_NAME googletest-release-${VERSION}