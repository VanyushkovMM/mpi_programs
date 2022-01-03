#pragma once
#ifndef VECTOR_ALTERNATIONS_H_
#define VECTOR_ALTERNATIONS_H_

void fillVectorRandValueWithoutZero(int* vec, int len);
void sequentialCount(const int* vec, int len, int& count);
int parallelCount(const int* vec, int len, int procCount);

#endif  // VECTOR_ALTERNATIONS_H_
