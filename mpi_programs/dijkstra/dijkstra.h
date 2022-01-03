#pragma once
#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

int* gen_graf(int size, int procRank = 0);
int* sequentialDijkstra(int* matrix, int size);
int* parallelDijkstra(int* matrix, int size);

#endif  // DIJKSTRA_H_
