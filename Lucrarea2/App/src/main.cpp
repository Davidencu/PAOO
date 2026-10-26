#include <iostream>

#include "mathlib.h"

int main() {
    std::cout << "2 + 3 = " << mathlib::add(2, 3) << '\n';
    std::cout << "4 * 5 = " << mathlib::multiply(4, 5) << '\n';
    std::cout << "5! = " << mathlib::factorial(5) << '\n';
    return 0;
}
