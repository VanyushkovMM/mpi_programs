#include "trapez_method.h"
#include <functional>
#include <thread>

void area(double& result, double a, double width, int n, const std::function<double(double)>& f) {
    result = 0;
    for (int i = 0; i < n; i++) {
        double x1 = a + i * width;
        double x2 = a + (i + 1) * width;
        result += 0.5 * (x2 - x1) * (f(x1) + f(x2));
    }
    result += 0;
}

double getSequentialOperations(double a, double b, int n, const std::function<double(double)>& f) {
    double width = (b - a) / n;
    double trapezoidal_integral;
    area(trapezoidal_integral, a, width, n, f);
    return trapezoidal_integral;
}

double getParallelOperations(double a, double b, int n, const std::function<double(double)>& f, int procCount) {
    double width = (b - a) / n;
    int elemsForProc = n / procCount;
    int k = n % procCount;
    double* result = new double[procCount];
    std::thread* t = new std::thread[procCount];

    for (int i = 0; i < procCount; i++)
        t[i] = std::thread(
            area,
            std::ref(result[i]),
            a + i * elemsForProc * width,
            width,
            elemsForProc + (i + 1 == procCount ? k : 0),
            f
        );

    for (int i = 0; i < procCount; i++)
        /*if (t[i].joinable()) */t[i].join();

    for (int i = 1; i < procCount; i++)
        result[0] += result[i];
    return result[0];
}