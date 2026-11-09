#include <cstddef>
#include <iostream>

class Buffer {
private:
    const int id_;   
    size_t size_;
    int* data_;    
public:

    // ITEM 4: Initializarea membrilor inainte de utilizare
    Buffer(int id, size_t size, int init_val)
        : id_(id),
          size_(size),
          data_(new int[size_]) 
    {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = init_val;
        }
        // id_ = id; // EROARE: assignment of read-only member 'Buffer::id_'
    }
    ~Buffer() {
        delete[] data_;
    }
    // ITEM 6: Interzicerea explicita a functiilor nedorite
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    void print() const {
        std::cout << "[Buffer " << id_ << "] Dimensiune: " << size_ 
                  << " | Adresa heap data_: " << data_ 
                  << " | Primul element: " << data_[0] << "\n";
    }
};


struct ImplicitDemo {
    const int fixed_id;
    int* ptr;
    ImplicitDemo(int id, int val) : fixed_id(id), ptr(new int(val)) {}
    ~ImplicitDemo() { delete ptr; }
    // Nu am declarat copy constructor sau operator
};

int main() {

    std::cout << "--- 1. Demonstrare Item 4 (Constructie si Initializare) ---\n";
    Buffer b1(101, 4, 7);
    b1.print();
    std::cout << "\n--- 2. Demonstrare Item 5 (Ce genereaza compilatorul in tacere) ---\n";
    ImplicitDemo d1(1, 42);

    // Compilatorul genereaza automat constructorul de copiere:

    ImplicitDemo d2 = d1; 
    std::cout << "d1.ptr indica spre: " << d1.ptr << "\n";
    std::cout << "d2.ptr indica spre: " << d2.ptr << " (acelasi pointer -> shallow copy!)\n";

    // d2 = d1; // EROARE (Item 5): use of implicitly deleted 'operator=' because of 'const int fixed_id'

    d2.ptr = nullptr;
    std::cout << "\n--- 3. Demonstrare Item 6 (Blocarea copierii prin = delete) ---\n";

    // Buffer b2 = b1; 
    // EROARE (Item 6): use of deleted function 'Buffer::Buffer(const Buffer&)'

    // Buffer b3(102, 2, 0);
    // b3 = b1;        
    // EROARE (Item 6): use of deleted function 'Buffer& Buffer::operator=(const Buffer&)'
    std::cout << "Bufferul b1 este protejat impotriva copierii accidentale.\n";
    return 0;
}
