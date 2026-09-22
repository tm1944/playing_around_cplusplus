#include <iostream>
#include <stdexcept>
#include "Vector.h"

template <class T>
Vector<T>::Vector(std::size_t capacity)
    : capacity_(capacity), data_(new T[capacity_]) {}

template <class T>
Vector<T>::~Vector(){
    delete[] data_;
}

template <class T>
std::size_t Vector<T>::size() const{
    return size_;
}

template<class T>
std::size_t Vector<T>::get_capacity()const{
    return capacity_;
}

template <class T>
bool Vector<T>::empty() const{
    if (size_ == 0){
        return true;
    }
    return false;
}

template <class T>
T Vector<T>::front() const{
    if(empty()){
        throw std::out_of_range("ERROR: Vector is empty!");
    }
    return data_[0];
}

template <class T>
T Vector<T>::back() const {
    if(empty()){
        throw std::out_of_range("Error: Vector is empty!");
    }
    return data_[size_ - 1];
}

template <class T>
void Vector<T>::copy_new_array_(T* new_data){
    std::size_t it = 0;
    while (it < size_){
        new_data[it] = data_[it];
        ++it;
    }
}


template <class T>
void Vector<T>::allocate_new_array(std::size_t capacity){
        //allocate new array
        T* new_data = new T[capacity];
        copy_new_array_(new_data);
        delete[] data_;
        data_ = new_data;
}

template <class T>
void Vector<T>::resize(std::size_t n){
    if (n <= capacity_){
        if (n > size_){
            for (std::size_t i = size_;i < n;++i){
                data_[i] = T{};
            }
        }
        size_ = n;
    }else if(n > capacity_){
        if (capacity_ == 0){
            capacity_++;
        }
        while(capacity_ < n){
            capacity_*=2;
        }
        
        allocate_new_array(capacity_);
        for (std::size_t i = size_;i < n;++i){
            data_[i] = T{};
        }
        size_ = n;
    }
}

template <class T>
void Vector<T>::push_back(T value){
    if(size_ == capacity_){
        if (capacity_ == 0){
            capacity_ = 1;
        }else{
            capacity_ *= 2;
        }
        allocate_new_array(capacity_);
    }
    data_[size_] = value;
    size_++;
}

template<class T>
void Vector<T>::reserve(std::size_t n){
    if(capacity_ < n){
        allocate_new_array(n);
        capacity_ = n;
    }
}

template <class T>
void Vector<T>::shrink_to_fit(){
    if(size_ == 0){
        size_ = 0;
        capacity_ = 0;
        delete[] data_;
        data_ = nullptr;
    }else if(size_ < capacity_){
        allocate_new_array(size_);
        capacity_ = size_;
    }
    
}


template <class T>
void Vector<T>::pop_back(){
    if(size_ == 0){
        throw std::out_of_range("Error: Vector is empty!");
    }
    size_--;
}

template <class T>
void Vector<T>::clear(){
    size_ = 0;
}