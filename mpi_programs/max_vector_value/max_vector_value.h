#pragma once
#ifndef MAX_VECTOR_VALUE_H_
#define MAX_VECTOR_VALUE_H_

void fillVectorRandValue(int* vec, int len);

int sequentialMax(const int* vec, int len);

int parallelMax(const int* vec, int len);

#endif  // MAX_VECTOR_VALUE_H_

