#include "mathlib.h"

namespace mathlib {

int add(int a, int b) { return a + b; }

int multiply(int a, int b) { return a * b; }

long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

}
