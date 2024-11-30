#!/bin/bash
set -e

mkdir -p build
g++ src/verifier.cpp test/*.cpp -o build/verifier