# EP-2022WS

This repository includes our project files for the _185.190 Efficient Programs 2022W_ lecture. The task was to optimise a given implementation of the program, which counts the number of _Ramanujan-Hardy Numbers_ - also known as _Taxicab Numbers_ - up to a given value n. Each valid number is counted only once (with regards to _N-Way Ramanujan Numbers_). The solution should be able to run using less than 100MB memory and is tested using 10^13 as an input. We were able to reduce the memory allocations down from ~3.7GB down to ~344KB and runtime from ~25.57s to ~9.28s on a provided system.

In order to automate the benchmarking our implementations, we wrote a short script, that can be invoked like this: `./bench_csv.sh -p [VERSION_DIRECTORY] -n [INPUT_SIZE] -r [REPEATS]`
