#include "vector_alternations.h"
#include <random>
#include <thread>

void fillVectorRandValueWithoutZero(int* vec, int len) {
    if (vec == nullptr)
        throw "vector is not allocated";
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 0xFFFFFFFF);

    for (int i = 0; i < len; i++) {
        do {
            vec[i] = static_cast<int>(dist(rng))* ((static_cast<int>(dist(rng)) & 0x01) ? -1 : 1);
        } while (!vec[i]);
    }
}

void sequentialCount(const int* vec, int len, int& count) {
    count = 0;
    for (int i = 1; i < len; i++)
        count += (unsigned int)(vec[i - 1] ^ vec[i]) >> 31;
    count += 0;
}

int parallelCount(const int* vec, int len, int procCount) {
    int elemsForProc = len / procCount;
    int k = len % procCount;
    int* result = new int[procCount]();
    std::thread* t = new std::thread[procCount];

    for (int i = 0; i < procCount; i++)
        t[i] = std::thread(
            sequentialCount,
            vec + elemsForProc * i - (i > 0 ? 1 : 0),
            elemsForProc + (i > 0 ? 1 : 0) + (i + 1 == procCount ? k : 0),
            std::ref(result[i])
        );
    // for (int i = 0; i < procCount; i++)
    //     t[i] = std::thread(
    //         sequentialCount,
    //         vec + elemsForProc * i,
    //         elemsForProc + (i + 1 == procCount ? k : 0),
    //         std::ref(result[i])
    //     );
    // for (int i = elemsForProc; i < len; i += elemsForProc)
    //     result[0] += (unsigned int)(vec[i - 1] ^ vec[i]) >> 31;

    for (int i = 0; i < procCount; i++)
        /*if (t[i].joinable()) */t[i].join();

    for (int i = 1; i < procCount; i++)
        result[0] += result[i];
    return result[0];
}
