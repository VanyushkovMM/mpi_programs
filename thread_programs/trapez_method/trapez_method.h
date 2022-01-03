#pragma once
#ifndef TRAPEZ_METHOD_H_
#define TRAPEZ_METHOD_H_

#include <functional>

double getSequentialOperations(double a, double b, int n, const std::function<double(double)>& f);
double getParallelOperations(double a, double b, int n, const std::function<double(double)>& f, int procCount);

#endif  // TRAPEZ_METHOD_H_

