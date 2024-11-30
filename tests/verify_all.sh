#!/bin/bash
set -e

for test_name in copy_prop cse dce lagrange matrix ray_trig rotation sobel; do
    echo "Testing $test_name"
    tests/run.sh tests/testcases/$test_name.cpp
    build/verifier $test_name noopt.dot 10000
    build/verifier $test_name opt.dot 10000
done