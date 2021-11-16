#include <mpi.h>
#include <iostream>
#include <functional>
#include <vector>
#include "max_vector_value.h"
#include "vector_alternations.h"
#include "trapez_method.h"

#define funcInt std::function<int(int*, const int)>
#define funcDouble std::function<double(double)>

template<typename T>
void checkResults(T sequential, T parallel)
{
    if (typeid(T) == typeid(int) && sequential == parallel || typeid(T) == typeid(double) && abs(sequential - parallel) < 1e-9)
        std::cout << "Success";
    else
    {
        std::cout << "Uncorrect result";
        std::cout << "\nSequential: " << sequential;
        std::cout << "\nParallel:   " << parallel;
    }
    std::cout << "\n\n";
}

void funcVec(const int vecSize, const char* text, const funcInt& sequentialF, const funcInt& parallelF)
{
    int procRank = 0;
    int* vec = nullptr;
    double start_time, end_time;
    int sequential, parallel;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        std::cout << text << ":\n";
        vec = new int[vecSize];
        fillVectorRandValue(vec, vecSize);

        start_time = MPI_Wtime();
        sequential = sequentialF(vec, vecSize);
        end_time = MPI_Wtime();
        std::cout << "Sequential: " << end_time - start_time << '\n';
    }

    start_time = MPI_Wtime();
    parallel = parallelF(vec, vecSize);
    if (procRank == 0) {
        end_time = MPI_Wtime();
        std::cout << "Parallel:   " << end_time - start_time << '\n';
        delete[] vec;
        checkResults<int>(sequential, parallel);
    }
}

void trapez_method(const char* text, const funcDouble& f, int n)
{
    int procRank = 0;
    double start_time, end_time;
    double sequential, parallel;
    double a = 0, b = 10;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0)
    {
        std::cout << text << ":\n";
        start_time = MPI_Wtime();
        sequential = getSequentialOperations(a, b, n, f);
        end_time = MPI_Wtime();
        std::cout << "Sequential: " << end_time - start_time << '\n';
    }

    start_time = MPI_Wtime();
    parallel = getParallelOperations(a, b, n, f);
    if (procRank == 0) {
        end_time = MPI_Wtime();
        std::cout << "Parallel:   " << end_time - start_time << '\n';
        checkResults<double>(sequential, parallel);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    const int n = atoi(argv[1]);

    const funcInt sM = funcInt(sequentialMax);
    const funcInt pM = funcInt(parallelMax);
    funcVec(n, "Max matrix value", sM, pM);

    const funcInt sC = funcInt(sequentialCount);
    const funcInt pC = funcInt(parallelCount);
    funcVec(n, "Vector alternations", sC, pC);

    const funcDouble x_polynom = [](double x) { return x * x - 5 * x + 4; };
    trapez_method("Polynom", x_polynom, n / 10);

    const funcDouble x_sin = [](double x) { return sin(x); };
    trapez_method("Sin(x)", x_polynom, n / 10);

    MPI_Finalize();
	return 0;
}