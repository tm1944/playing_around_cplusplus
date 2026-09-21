#include <iostream>
#include <stdexcept>
#include "Vector.h"

template <class T>
Vector<T>::Vector(){}

template <class T>
Vector<T>::~Vector(){}

template <class T>
std::size_t Vector<T>::size() const{
    return size_;
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
void Vector<T>::push_back(T value){
    if(size_ >= CAPACITY){
        throw std::length_error("Error: Vector Capacity already reached!");
    }

    data_[size_] = value;
    size_++;
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