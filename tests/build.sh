#!/bin/bash
set -e

cmake -S src/DataflowGenerator -B build
cmake --build build

g++ src/verifier.cpp tests/testcases/*.cpp -o build/verifier