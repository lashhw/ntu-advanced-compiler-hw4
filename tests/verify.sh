#!/bin/bash
set -e

mkdir -p build
g++ src/verifier.cpp tests/testcases/*.cpp -o build/verifier