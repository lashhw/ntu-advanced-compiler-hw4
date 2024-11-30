#!/bin/bash
set -e

if [ "$#" -ne 1 ]; then
    echo "usage: $0 source_file"
    exit
fi

cmake -S src -B build
cmake --build build

clang -Xclang -disable-O0-optnone -fno-discard-value-names -ffp-contract=off -O0 -S -emit-llvm $1 -o ir.ll
opt -S -passes="mem2reg" ir.ll -o ir_noopt.ll
opt -load-pass-plugin build/DataflowAnalyzerPass.so -passes=dataflow-analyzer --disable-output ir_noopt.ll 2> ir_noopt.dot

clang -fno-discard-value-names -ffp-contract=off -O3 -fno-slp-vectorize -S -emit-llvm $1 -o ir_opt.ll
opt -load-pass-plugin build/DataflowAnalyzerPass.so -passes=dataflow-analyzer --disable-output ir_opt.ll 2> ir_opt.dot