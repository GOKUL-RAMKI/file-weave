#pragma once

#include <cmath>
#include <cstdio>
#include <cstdlib>

inline void check(bool ok, const char* name) {
    if (!ok) {
        std::fprintf(stderr, "FAIL: %s\n", name);
        std::exit(1);
    }
}

inline void checkNear(double actual, double expected, const char* name) {
    if (std::fabs(actual - expected) > 1e-9) {
        std::fprintf(stderr, "FAIL: %s (got %.6f, expected %.6f)\n", name, actual, expected);
        std::exit(1);
    }
}
