#pragma once
#ifndef TRAPEZ_METHOD_H_
#define TRAPEZ_METHOD_H_

#include <functional>

double getParallelOperations(double a, double b, int n, const std::function<double(double)>& f);
double getSequentialOperations(double a, double b, int n, const std::function<double(double)>& f);

#endif  // TRAPEZ_METHOD_H_


