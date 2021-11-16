#include "max_vector_value.h"
#include <mpi.h>
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

int sequentialMax(const int* vec, int len) {
    int maxValue = vec[0];
    for (int i = 1; i < len; i++) {
        if (vec[i] > maxValue) {
            maxValue = vec[i];
        }
    }
    return maxValue;
}

int parallelMax(const int* vec, int len) {
    int procCount = 0, procRank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    int elemsForProc = len / procCount + (procRank < len % procCount ? 1 : 0);

    int* scnts = nullptr, * displs = nullptr;
    int* rcount = new int[elemsForProc];
    if (procRank == 0) {
        scnts = new int[procCount];
        for (int i = 0; i < procCount; scnts[i++] = len / procCount) {}
        for (int i = 0; i < len % procCount; ++scnts[i++]) {}
        displs = new int[procCount];
        displs[0] = 0;
        for (int i = 1; i < procCount; i++) {
            displs[i] = displs[i - 1] + scnts[i - 1];
        }
    }

    MPI_Scatterv(reinterpret_cast<const void*>(vec), scnts, displs,
        MPI_INT, rcount, elemsForProc, MPI_INT, 0, MPI_COMM_WORLD);

    int localAlternations = sequentialMax(rcount, elemsForProc);
    int globalAlterations = 0;
    MPI_Reduce(reinterpret_cast<void*>(&localAlternations), reinterpret_cast<void*>(&globalAlterations),
        1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (procRank == 0) {
        delete[] scnts;
        delete[] displs;
    }

    delete[] rcount;
    return globalAlterations;
}