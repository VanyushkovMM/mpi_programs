#include "dijkstra.h"
#include <random>
#include <thread>

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
void get_dijkstra(int* distance, int* matrix, int size, int t) {
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
}

void run(int* vec, int* matrix, int size, int t0, int t1) {
    int* distance = new int[size];
    for (int t = t0; t < t1; t++) {
        int k = t * size;
        get_dijkstra(distance, matrix, size, t);
        for (int i = 0; i < size; i++) {
            vec[k + i] = distance[i];
        }
    }
    delete[] distance;
}

int* sequentialDijkstra(int* matrix, int size) {
    int* vec = new int[size * size];
    run(vec, matrix, size, 0, size);
    return vec;
}

int* parallelDijkstra(int* matrix, int size, int procCount) {
    int* vec = new int[size * size];
    std::thread* t = new std::thread[procCount];

    int* scnts = new int[procCount];
    for (int i = 0; i < procCount; scnts[i++] = size / procCount) {}
    for (int i = 0; i < size % procCount; ++scnts[i++]) {}
    int* displs = new int[procCount + 1];
    displs[0] = 0;
    displs[procCount] = size;
    for (int i = 1; i < procCount; i++) {
        displs[i] = displs[i - 1] + scnts[i - 1];
    }
    delete[] scnts;

    for (int i = 0; i < procCount; i++)
        t[i] = std::thread(
            run,
            vec,
            matrix,
            size,
            displs[i],
            displs[i + 1]
        );

    for (int i = 0; i < procCount; i++)
        if (t[i].joinable()) t[i].join();
    delete[] displs;

    return vec;
}
