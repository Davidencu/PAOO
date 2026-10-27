#include "DynamicArray.hpp"
#include <iostream>
#include <utility> 

DynamicArray createTempArray() {
    DynamicArray temp(3, 42);
    return temp;
}

int main() {
    std::cout << "1. & 2. Inapsulare & Constructor Initializer List\n";
    DynamicArray arr1(3, 10);
    arr1.set(0, 99);
    arr1.print();

    std::cout << "\n3. Demonstratie Copy Constructor (De ce e necesar)\n";
    {
        DynamicArray arr2 = arr1; 
        arr2.set(1, 555);

        std::cout << "arr1: "; arr1.print();
        std::cout << "arr2: "; arr2.print();
    }
    std::cout << "arr2 a fost distrus. arr1 este inca valid si izolat:\n";
    arr1.print();

    std::cout << "\n4. Demonstratie Move Constructor\n";
    DynamicArray arr3 = std::move(arr1);
    std::cout << "arr3 a preluat datele:\n";
    arr3.print();

    std::cout << "\n5. Move dintr-un obiect temporar (rvalue)\n";
    DynamicArray arr4 = createTempArray();
    arr4.print();

    std::cout << "\nIesire din main (curatare automata prin destructori)\n";
    return 0;
}