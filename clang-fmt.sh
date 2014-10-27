#!/bin/bash

set -o nounset
set -o errexit
set -o pipefail

# I'm running this on Mac. To install the tools:
# brew install dos2unix
# clang-format comes with XCode 6.1 (I think)
# It's in /usr/local/bin/clang-format

cd src
clang-format -style="{BasedOnStyle: Mozilla, IndentWidth: 4}" -i *.cpp *.h
dos2unix -s -v *.cpp *.h
