#pragma once

#include <cstddef>

template <class T>
class Vector {
public:
    explicit Vector(std::size_t capacity);
    Vector(const Vector<T>& other);
    Vector<T>& operator=(const Vector<T>& other);
    ~Vector();

    std::size_t size() const;
    bool empty() const;
    std::size_t get_capacity() const;
    const T& front() const;
    const T& back() const;
    void reserve(std::size_t n);
    void push_back(T value);
    void pop_back();
    void clear();
    void shrink_to_fit();
    void resize(std::size_t n);

private:
    std::size_t capacity_{};
    T* data_ = nullptr;
    std::size_t size_{};

    void reallocate_(std::size_t capacity);
    void grow_();
};

#include "Vector.cpp"
