#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <cstddef>

class DynamicArray {
private:
    size_t m_size; 
    int* m_data; 

public:
    DynamicArray(size_t size, int initialValue = 0);
    ~DynamicArray();
    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;

    size_t getSize() const;
    void set(size_t index, int value);
    int get(size_t index) const;
    void print() const;
};

#endif 