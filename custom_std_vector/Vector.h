#pragma once

#include <cstddef>
#include <memory>

template <class T>
class Vector {
public:
    explicit Vector(std::size_t capacity);//default constructor Vector<T> v {n}; explicit doesn't allow Vector<T> v = 10;
    Vector(const Vector<T>& other);
    Vector<T>& operator=(const Vector<T>& other); //creates a deep copy of another vector
    ~Vector(); //destructor
    Vector(Vector<T>&& other) noexcept; //called when constructing a new vector from a moveable vector &&

    std::size_t size() const; //returns size of vector
    bool empty() const; //returns if vector is empty or not
    std::size_t get_capacity() const; //returns capacity 
    const T& front() const; //returns a const reference first index element
    const T& back() const; //returns a const reference last index element

    Vector<T>& operator=(Vector<T>&& other) noexcept; //noexcept: promises to not throw an exception

    //throws exception for out of bounds
    const T& at(std::size_t index) const; //returns const reference to element at index
    T& at(std::size_t index); //returns a reference to element at index

    //same as at() but doesn't do bounds checking
    const T& operator[](std::size_t index) const;
    T& operator[](std::size_t index);

    void reserve(std::size_t n); // ensures capacity of atleast n
    void push_back(T value); //add element to last index
    void pop_back(); //pops element at last index
    void clear(); //clears the entire vector
    void shrink_to_fit(); //shrinks capacity down to size
    void resize(std::size_t n); //changes number of elements to n


private:
    std::allocator<T> allocator_;
    std::size_t capacity_{};
    T* data_ = nullptr;
    std::size_t size_{};

    void reallocate_(std::size_t capacity);
    void grow_();
};

#include "Vector.cpp"
