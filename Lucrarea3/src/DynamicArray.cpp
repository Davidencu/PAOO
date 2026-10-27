#include "DynamicArray.hpp"
#include <iostream>

DynamicArray::DynamicArray(size_t size, int initialValue)
    : m_size(size), m_data(new int[size]) 
{
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] = initialValue;
    }
    std::cout << "[Constructor] Array de marime " << m_size << " creat la adresa: " << m_data << "\n";
}

DynamicArray::~DynamicArray() {
    std::cout << "[Destructor] Array la adresa " << m_data << " distrus.\n";
    delete[] m_data;
    m_data = nullptr;
}

DynamicArray::DynamicArray(const DynamicArray& other)
    : m_size(other.m_size), m_data(new int[other.m_size]) 
{
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] = other.m_data[i];
    }
    std::cout << "[Copy Constructor] Deep copy creat la noua adresa: " << m_data 
              << " (duplicat de la " << other.m_data << ")\n";
}

DynamicArray::DynamicArray(DynamicArray&& other) noexcept
    : m_size(other.m_size), m_data(other.m_data) 
{
    other.m_size = 0;
    other.m_data = nullptr; 
    std::cout << "[Move Constructor] Resursa preluata la adresa: " << m_data << "\n";
}

size_t DynamicArray::getSize() const { 
    return m_size; 
}

void DynamicArray::set(size_t index, int value) {
    if (index < m_size) {
        m_data[index] = value;
    }
}

int DynamicArray::get(size_t index) const {
    if (index < m_size) {
        return m_data[index];
    }
    return -1;
}

void DynamicArray::print() const {
    std::cout << "Data: [ ";
    for (size_t i = 0; i < m_size; ++i) {
        std::cout << m_data[i] << " ";
    }
    std::cout << "]\n";
}