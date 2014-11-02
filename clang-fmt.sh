#!/bin/bash

set -o nounset
set -o errexit
set -o pipefail

# I'm running this on Mac. To install the tools:
# brew install dos2unix
# I installed clang-format by downloading binary build from http://llvm.org/releases/download.html
# and doing:
# cp ~/Downloads/clang+llvm-3.5.0-macosx-apple-darwin/bin/clang-format /usr/local/bin
# so that it ends up in /usr/local/bin/clang-format

cd src
clang-format -style="{BasedOnStyle: Mozilla, IndentWidth: 4, ColumnLimit: 120}" -i *.cpp *.h
dos2unix -s -v *.cpp *.h
