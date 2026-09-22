
template <class T>
class Vector{
public:
    Vector(std::size_t capacity);
    ~Vector();
    std::size_t size() const;
    bool empty() const;
    std::size_t get_capacity() const;
    T front() const;
    T back() const;
    void reserve(std::size_t n);
    void push_back(T value);
    void pop_back();
    void clear();
    void shrink_to_fit();
    void resize(std::size_t n);

private:
    std::size_t capacity_ {};
    T* data_ = nullptr;
    std::size_t size_ {};

    void copy_new_array_(T* new_data);
    void allocate_new_array(std::size_t capacity);
};

#include "Vector.cpp"