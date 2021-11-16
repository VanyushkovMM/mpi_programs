#include "vector_alternations.h"
#include <mpi.h>
#include <random>

int sequentialCount(const int* vec, int len) {
    int count = 0;
    for (int i = 1; i < len; i++)
        count += (unsigned int)(vec[i - 1] ^ vec[i]) >> 31;
    return count;
}

void fillVecWithRandValues(int* vec, int len) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 0xFFFFFFFF);
    if (vec == nullptr)
        throw "vector is not allocated";
    for (int i = 0; i < len; i++) {
        do {
            vec[i] = static_cast<int>(dist(rng))* ((static_cast<int>(dist(rng)) & 0x01) ? -1 : 1);
        } while (!vec[i]);
    }
}


int parallelCount(const int* vec, int len) {
    int procCount = 0, procRank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    if (len < procCount * 2) 
        return (procRank == 0) ? sequentialCount(vec, len) : 0;

    int elemsForProc = len / procCount;  // 11 elems, 4 proc -> [2,2,2,2] | 10e, 4p -> [2,2,2,2]
    if (procRank < len % procCount)
        elemsForProc++;  // [2,2,2,2] -> [3,3,3,2] | [2,2,2,2] -> [3,3,2,2]
    if (procRank < procCount - 1)
        elemsForProc++;  // [3,3,3,2] -> [4,4,4,2] | [3,3,2,2] -> [4,4,3,2]
    int* sendcounts = nullptr, * displs = nullptr;
    int* recvbuf = new int[elemsForProc];
    if (procRank == 0) {
        sendcounts = new int[procCount];
        displs = new int[procCount];
        std::fill_n(sendcounts, procCount, len / procCount);  // scnt[?,?,?,?]->[2,2,2,2]
        for (int i = 0; i < len % procCount; ++sendcounts[i++]) {}  // scnt[2,2,2,2]->[3,3,3,2] | [2,2,2,2]->[3,3,2,2]
        displs[0] = 0;  // dspls[0,?,?,?]
        for (int i = 1; i < procCount; i++)
            displs[i] = displs[i - 1] + sendcounts[i - 1];  // dspls[0,?,?,?]->[0,3,6,9] | [0,?,?,?]->[0,3,6,8]
        for (int i = 0; i < procCount - 1; i++)
            sendcounts[i]++;  // scnt[3,3,3,2]->[4,4,4,2] | [3,3,2,2]->[4,4,3,2]
        // in total 11e 4p scnt[4,4,4,2] dspls[0,3,6,9] | 10e 4p s[4,4,3,2] d[0,3,6,8]
    }
    MPI_Scatterv(reinterpret_cast<const void*>(vec), sendcounts, displs,
        MPI_INT, recvbuf, elemsForProc, MPI_INT, 0, MPI_COMM_WORLD);

    int localAlternations = sequentialCount(recvbuf, elemsForProc);
    int globalAlterations = 0;
    MPI_Reduce(reinterpret_cast<void*>(&localAlternations), reinterpret_cast<void*>(&globalAlterations),
        1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (procRank == 0) {
        delete[] sendcounts;
        delete[] displs;
    }
    delete[] recvbuf;
    return globalAlterations;
}
