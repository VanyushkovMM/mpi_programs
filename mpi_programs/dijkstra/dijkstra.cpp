#include "dijkstra.h"
#include <random>
#include <mpi.h>

#define MAX_INT_VALUE 0x0FFFFFFF

int* gen_graf(int size, int procRank) {
    int* matrix = new int[size * size];
    if (procRank == 0) {
        // https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distInt(0, 0x0FFFFFFE / size);

        for (int i = 0; i < size; i++) {
            matrix[i * size + i] = 0;
            for (int j = i + 1; j < size; j++) {
                matrix[i * size + j] = static_cast<int>(distInt(rng));
                matrix[j * size + i] = matrix[i * size + j];
            }
        }
    }
    return matrix;
}

// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
int* get_dijkstra(int* matrix, int size, int t) {
    int* distance = new int[size];
    bool* visited = new bool[size];
    for (int i = 0; i < size; i++) {
        distance[i] = MAX_INT_VALUE;
        visited[i] = false;
    }
    distance[t] = 0;
    int min_dist = 0;
    int min_vertex = t;

    int temp_dist, i;
    while (min_dist < MAX_INT_VALUE) {
        i = min_vertex;
        visited[i] = true;
        for (int j = 0; j < size; j++) {
            temp_dist = distance[i] + matrix[i * size + j];
            if (temp_dist < distance[j] && matrix[i * size + j] != 0) {
                distance[j] = temp_dist;
            }
            min_dist = MAX_INT_VALUE;
            for (int j = 0; j < size; j++) {
                if (!visited[j] && distance[j] < min_dist) {
                    min_dist = distance[j];
                    min_vertex = j;
                }
            }
        }
    }
    delete[] visited;
    return distance;
}

int* sequentialDijkstra(int* matrix, int size) {
    int* vec = new int[size * size];
    for (int t = 0; t < size; t++) {
        int k = t * size;
        int* t_res = get_dijkstra(matrix, size, t);
        for (int i = 0; i < size; i++) {
            vec[k + i] = t_res[i];
        }
        delete[] t_res;
    }
    return vec;
}

int* parallelDijkstra(int* matrix, int size) {
    int procRank, procCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);

    int countTopForProc = size / procCount + (procRank < size % procCount ? 1 : 0);
    int firstTopForProc = 0;
    for (int i = 0; i < procRank; i++) {
        firstTopForProc += size / procCount + (i < size % procCount ? 1 : 0);
    }

    int* vec = new int[size * size];
    int* vecForProc = new int[size * countTopForProc];

    int* scnts = nullptr, * displs = nullptr;
    if (procRank == 0) {
        scnts = new int[procCount];
        for (int i = 0; i < procCount; scnts[i++] = size / procCount) {}
        for (int i = 0; i < size % procCount; ++scnts[i++]) {}
        for (int i = 0; i < procCount; i++) {
            scnts[i] *= size;
        }
        displs = new int[procCount];
        displs[0] = 0;
        for (int i = 1; i < procCount; i++) {
            displs[i] = displs[i - 1] + scnts[i - 1];
        }
    }

    MPI_Bcast(reinterpret_cast<void*>(matrix), size * size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int t = 0; t < countTopForProc; t++) {
        int k = t * size;
        int* tmpVec = get_dijkstra(matrix, size, firstTopForProc + t);
        for (int i = 0; i < size; i++) {
            vecForProc[k + i] = tmpVec[i];
        }
        delete[] tmpVec;
    }
    MPI_Gatherv(reinterpret_cast<void*>(vecForProc), size * countTopForProc, MPI_INT,
        reinterpret_cast<void*>(vec), scnts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (procRank == 0) {
        delete[] scnts;
        delete[] displs;
    }
    delete[] vecForProc;

    return vec;
}
