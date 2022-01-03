#pragma once
#ifndef MAX_VECTOR_VALUE_H_
#define MAX_VECTOR_VALUE_H_

void fillVectorRandValue(int* vec, int len);
void sequentialMax(const int* vec, int len, int& maxValue);
int parallelMax(const int* vec, int len, int procCount);

#endif  // MAX_VECTOR_VALUE_H_

