#pragma once

#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace std;

inline void check(bool ok, const char* name) {
    if (!ok) {
        fprintf(stderr, "FAIL: %s\n", name);
        exit(1);
    }
}

inline void checkNear(double actual, double expected, const char* name) {
    if (fabs(actual - expected) > 1e-9) {
        fprintf(stderr, "FAIL: %s (got %.6f, expected %.6f)\n", name, actual, expected);
        exit(1);
    }
}
