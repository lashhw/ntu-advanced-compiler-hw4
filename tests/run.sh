#!/bin/bash
set -e

if [ "$#" -ne 1 ]; then
    echo "usage: $0 <source_file>"
    exit
fi

clang -Xclang -disable-O0-optnone -fno-discard-value-names -ffp-contract=off -O0 -S -emit-llvm $1 -o - | opt -S -passes="mem2reg" -o noopt.ll
opt -load-pass-plugin build/DataflowGeneratorPass.so -passes=dataflow-generator --disable-output noopt.ll 2> noopt.dot

clang -fno-discard-value-names -ffp-contract=off -O3 -fno-slp-vectorize -S -emit-llvm $1 -o opt.ll
opt -load-pass-plugin build/DataflowGeneratorPass.so -passes=dataflow-generator --disable-output opt.ll 2> opt.dot