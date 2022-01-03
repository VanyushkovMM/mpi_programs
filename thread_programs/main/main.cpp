#include <iostream> 
#include <thread>
#include <ctime>
#include "max_vector_value.h"
#include "trapez_method.h"
#include "vector_alternations.h"
#include "dijkstra.h"

#define funcVoid std::function<void(int*, const int, int&)>
#define funcInt std::function<int(int*, const int, int)>
#define funcDouble std::function<double(double)>

template<typename T1, typename T2>
void checkResults(const char* text, T1 sRes, T1 pRes, T2 sTime, T2 pTime)
{
    std::cout << text << ":\n";
    std::cout << "Sequential: " << sTime << '\n';
    std::cout << "Parallel:   " << pTime << '\n';

    if (typeid(T1) == typeid(int) && sRes == pRes 
        || typeid(T1) == typeid(double) && abs(sRes - pRes) < 1e-5)
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
    if (typeid(T1) == typeid(int) && sRes == pRes || typeid(T1) == typeid(double) && abs(sRes - pRes) < 1e-5)
        std::cout << sTime << ' ' << pTime << '\n';
    else
        std::cout << "-1" << '\n';
}

void funcVec(
    const int procCount, 
    const int vecSize, 
    const bool flag, 
    const char* text, 
    const funcVoid& sequentialF, 
    const funcInt& parallelF) 
{
    unsigned start_time, end_time;
    unsigned sTime, pTime;
    int sequential, parallel;

    int* vec = new int[vecSize];
    fillVectorRandValue(vec, vecSize);

    start_time = clock();
    sequentialF(vec, vecSize, sequential);
    end_time = clock();
    sTime = end_time - start_time;

    start_time = clock();
    parallel = parallelF(vec, vecSize, procCount);
    end_time = clock();
    pTime = end_time - start_time;

    delete[] vec;
    if (flag) checkResults<int, unsigned>(text, sequential, parallel, sTime, pTime);
    else returnTime<int, unsigned>(sequential, parallel, sTime, pTime);
}

void trapez_method(
    const int procCount, 
    const int n, 
    const bool flag, 
    const char* text, 
    const funcDouble& f)
{
    unsigned start_time, end_time;
    unsigned sTime, pTime;
    double sequential, parallel;
    double a = 0, b = 19.9 * (n - 1) / (1e8 - 1) + 0.1;

    start_time = clock();
    sequential = getSequentialOperations(a, b, n, f);
    end_time = clock();
    sTime = end_time - start_time;

    start_time = clock();
    parallel = getParallelOperations(a, b, n, f, procCount);
    end_time = clock();
    pTime = end_time - start_time;

    if (flag) checkResults<double, unsigned>(text, sequential, parallel, sTime, pTime);
    else returnTime<double, unsigned>(sequential, parallel, sTime, pTime);
}

void dijkstra(
    const int procCount,
    const int n,
    const char* text) 
{
    int size = 2;
    int x = 10;
    while (true) {
        if (x < n) size += 38;
        else break;
        x *= 10;
    }
    // int size = 1.98 * (n < 100000 ? n / 1000 : 100) + 2;

    unsigned start_time, end_time;
    unsigned sTime, pTime;
    int* seq, * par;
    int sequential = 1, parallel = 1;

    int* graf = gen_graf(size);

    start_time = clock();
    par = parallelDijkstra(graf, size, procCount);

    end_time = clock();
    pTime = end_time - start_time;

    start_time = clock();
    seq = sequentialDijkstra(graf, size);
    end_time = clock();
    sTime = end_time - start_time;

    size *= size;
    for (int i = 0; i < size; i++)
        if (seq[i] != par[i]) {
            parallel = 0; break;
        }

    checkResults<int, unsigned>(text, sequential, parallel, sTime, pTime);
    
    delete[] graf;
    delete[] seq;
    delete[] par;
}

int main(int argc, char** argv) {
    const int n = (argc > 1 ? atoi(argv[1]) : 100000);
    const int procCount = (argc > 2 ? atoi(argv[2]) : 4);
    const bool flag = (argc > 3 ? bool(atoi(argv[3])) : true);
    // const int n = 1000000;
    // const int procCount = 4;
    // const bool flag = true;

    const funcVoid sM = funcVoid(sequentialMax);
    const funcInt pM = funcInt(parallelMax);
    funcVec(procCount, n, flag, "Max matrix value", sM, pM);

    const funcVoid sC = funcVoid(sequentialCount);
    const funcInt pC = funcInt(parallelCount);
    funcVec(procCount, n, flag, "Vector alternations", sC, pC);

    const funcDouble x_polynom = [](double x) { return x * x - 5 * x + 4; };
    trapez_method(procCount, n, flag, "Polynom", x_polynom);

    dijkstra(procCount, n, "Dijkstra");

    return 0;
}