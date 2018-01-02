# Minimum-Vertex-Cover-CSE6140-2017-Fall




## Environment and Prerequisites
C++ and requires boost and c++11 library

## Overall Structure
1. RunMVC (.cpp/.h): program starting point, handles the input arguments and algorithm selection.    
2. Graph (.cpp/.h): Graph and Edge relating functions.
3. BnB (.cpp/h): Branch-and-bound algorithm
4. LS1 (.cpp/h): Local Search - Hill Climbing algorithm
5. LS2 (.cpp/h): Local Search - Simulated Annealing algorithm
6. Approx_DFS (.cpp/h): DFS Approximation algorithm

## MY part
1. LS1 -- Hill Climbing
2. LS2 -- Simulated Annealing
3. Part of Graph and RunMVC template

## Compile
Type 'make proj' in code directory, this will generate an executable file named 'exec'

## Execution
exec -inst [Data Directory] -alg [BnB|Approx|LS1|LS2] -time [seconds] -seed [value]

> Time indicates the maximum execution time, no matter the algorithm finds a
> promising result or not
>
> seed is for randomness, the same seed will generate the same results
