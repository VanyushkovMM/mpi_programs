# Parallel programs
Testing parallel programs with different amounts of processed data and number of processes.

Tasks used for comparison:
1. Maximum value of vector elements (directory `max_vector_value`)
2. The number of sign changes to the opposite in the adjacent elements of the vector (directory `vector_alternations`)
3. Finding the area of a polynomial graph by the trapezoid method (directory `trapez_method`)
4. Finding the shortest path between the vertices of a graph by dijkstra's algorithm (directory `dijkstra`)

Parallelization methods used:
1. Using MPI technology (directory `mpi_programs`)
2. Using c++ standard 11 multithreading technology (directory `thread_programs`)

Additional scripts:
* `build_programs.py` - helps to build a project quickly
* `test_programs.py` - runs tests and generates a table with output data
* `test.py` in directories `mpi_programs` and `thread_programs` - runs tests and prints results
* `others/run_tests.py` - runs tests from a file `test_arguments.txt` and prints results

Additional text files:
* `others/test_arguments.txt` - this file stores the input data for test_programs (each line indicates the number of processes and the number of processed elements)
* `results.txt` - table of test results (the average value of 15 runs is taken)
