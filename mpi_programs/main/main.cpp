#include <mpi.h>
#include <iostream>
#include <functional>
#include <vector>
#include "max_vector_value.h"
#include "vector_alternations.h"
#include "trapez_method.h"

#define funcInt std::function<int(int*, const int)>
#define funcDouble std::function<double(double)>

template<typename T1, typename T2>
void checkResults(const char* text, T1 sRes, T1 pRes, T2 sTime, T2 pTime)
{
    std::cout << text << ":\n";
    std::cout << "Sequential: " << sTime << '\n';
    std::cout << "Parallel:   " << pTime << '\n';

    if (typeid(T1) == typeid(int) && sRes == pRes 
        || typeid(T1) == typeid(double) && abs(sRes - pRes) < 1e-9)
        std::cout << "Success";
    else
    {
        std::cout << "Uncorrect result";
        std::cout << "\nSequential: " << sRes;
        std::cout << "\nParallel:   " << pRes;
    }
    std::cout << "\n\n";
}

template<typename T1, typename T2>
void returnTime(T1 sRes, T1 pRes, T2 sTime, T2 pTime)
{
    if (typeid(T1) == typeid(int) && sRes == pRes || typeid(T1) == typeid(double) && abs(sRes - pRes) < 1e-9)
        std::cout << sTime << ' ' << pTime << '\n';
    else
        std::cout << "-1" << '\n';
}

void funcVec(
    const int vecSize, 
    const bool flag, 
    const char* text, 
    const funcInt& sequentialF, 
    const funcInt& parallelF)
{
    int procRank = 0;
    int* vec = nullptr;
    double start_time, end_time;
    unsigned sTime, pTime;
    int sequential, parallel;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        std::cout << text << ":\n";
        vec = new int[vecSize];
        fillVectorRandValue(vec, vecSize);

        start_time = MPI_Wtime();
        sequential = sequentialF(vec, vecSize);
        end_time = MPI_Wtime();
        sTime = unsigned((end_time - start_time) * 1000);
    }

    start_time = MPI_Wtime();
    parallel = parallelF(vec, vecSize);
    if (procRank == 0) {
        end_time = MPI_Wtime();
        pTime = unsigned((end_time - start_time) * 1000);
        delete[] vec;

        if (flag) checkResults<int, unsigned>(text, sequential, parallel, sTime, pTime);
        else returnTime<int, unsigned>(sequential, parallel, sTime, pTime);
    }
}

void trapez_method(
    const int n, 
    const bool flag, 
    const char* text, 
    const funcDouble& f)
{
    int procRank = 0;
    double start_time, end_time;
    unsigned sTime, pTime;
    double sequential, parallel;
    double a = 0, b = (n < 10000 ? n / 1000 : 10);

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0)
    {
        std::cout << text << ":\n";
        start_time = MPI_Wtime();
        sequential = getSequentialOperations(a, b, n, f);
        end_time = MPI_Wtime();
        sTime = unsigned((end_time - start_time) * 1000);
    }

    start_time = MPI_Wtime();
    parallel = getParallelOperations(a, b, n, f);
    if (procRank == 0) {
        end_time = MPI_Wtime();
        pTime = unsigned((end_time - start_time) * 1000);

        if (flag) checkResults<double, unsigned>(text, sequential, parallel, sTime, pTime);
        else returnTime<double, unsigned>(sequential, parallel, sTime, pTime);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    const int n = atoi(argv[1]);
    const bool flag = (argc > 2 ? bool(atoi(argv[2])) : true);

    const funcInt sM = funcInt(sequentialMax);
    const funcInt pM = funcInt(parallelMax);
    funcVec(n, flag, "Max matrix value", sM, pM);

    const funcInt sC = funcInt(sequentialCount);
    const funcInt pC = funcInt(parallelCount);
    funcVec(n, flag, "Vector alternations", sC, pC);

    const funcDouble x_polynom = [](double x) { return x * x - 5 * x + 4; };
    trapez_method(n, flag, "Polynom", x_polynom);

    const funcDouble x_sin = [](double x) { return sin(x); };
    trapez_method(n, flag, "Sin(x)", x_polynom);

    MPI_Finalize();
	return 0;
}