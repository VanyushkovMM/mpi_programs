#include "max_vector_value.h"
#include <thread>
#include <random>

void fillVectorRandValue(int* vec, int len) {
    if (vec == nullptr)
        throw "vector is not allocated";
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distInt(0, 0xFFFFFFFF);
    for (int i = 0; i < len; i++) {
        vec[i] = static_cast<int>(distInt(rng));
    }
}

void sequentialMax(const int* vec, int len, int& maxValue) {
    maxValue = vec[0];
    for (int i = 1; i < len; i++) {
        if (vec[i] > maxValue) {
            maxValue = vec[i];
        }
    }
}

int parallelMax(const int* vec, int len, int procCount) {
    int elemsForProc = len / procCount;
    int k = len % procCount;
    int* result = new int[procCount]();
    std::thread* t = new std::thread[procCount];

    for (int i = 0; i < procCount; i++)
        t[i] = std::thread(
            sequentialMax, 
            vec + elemsForProc * i,
            elemsForProc + (i + 1 == procCount ? k : 0), 
            std::ref(result[i])
        );

    for (int i = 0; i < procCount; i++)
        /*if (t[i].joinable()) */t[i].join();

    sequentialMax(result, procCount, std::ref(result[0]));

    return result[0];
}