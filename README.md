# Advanced Compiler HW4

## 1. Environment

Before starting, ensure you are working on the `ws1` so that all the required tools and dependencies are satisfied.

## 2. Build the Project

Before executing any commands, compile the source files by running:

```bash
tests/build.sh
```

## 3. Generate a Dataflow Graph

To generate a dataflow graph for a specific source file, use:

```bash
tests/run.sh <source_file>
```

For example:
```bash
tests/run.sh tests/testcases/cse.cpp
```

The resulting dataflow graphs will be saved as `noopt.dot` (unoptimized) and `opt.dot` (optimized) in your current working directory.  
Additional test cases can be found under the `tests/testcases` directory.

## 4. Convert Dataflow Graphs to PNG Files

Ensure that `graphviz` is installed on your system. Then, convert `.dot` files to `.png` using:

```bash
dot -Tpng noopt.dot -o noopt.png
dot -Tpng opt.dot -o opt.png
```

## 5. Verify Correctness for All Test Cases

Check the correctness of all test cases by running:

```bash
tests/verify_all.sh
```

This script will validate whether the generated `.dot` files match the program semantics by randomly assigning inputs and comparing the outputs.

## 6. Clean Up

To remove generated files, use:

```bash
tests/clean.sh
```
