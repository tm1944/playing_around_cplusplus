#include <stdexcept>

template <class T>
Vector<T>::Vector(std::size_t capacity)
    : capacity_(capacity), data_(new T[capacity_]) {}

template <class T>
Vector<T>::Vector(const Vector<T>& other)
    : capacity_(other.capacity_), data_(new T[other.capacity_]), size_(other.size_) {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this == &other) {
        return *this;
    }

    T* new_data = new T[other.capacity_];
    for (std::size_t i = 0; i < other.size_; ++i) {
        new_data[i] = other.data_[i];
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = other.capacity_;
    size_ = other.size_;
    return *this;
}

template <class T>
Vector<T>::~Vector() {
    delete[] data_;
}

template <class T>
std::size_t Vector<T>::size() const {
    return size_;
}

template <class T>
std::size_t Vector<T>::get_capacity() const {
    return capacity_;
}

template <class T>
bool Vector<T>::empty() const {
    return size_ == 0;
}

template <class T>
const T& Vector<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Error: Vector is empty!");
    }
    return data_[0];
}

template <class T>
const T& Vector<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Error: Vector is empty!");
    }
    return data_[size_ - 1];
}

template <class T>
void Vector<T>::reallocate_(std::size_t new_capacity) {
    T* new_data = new T[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

template <class T>
void Vector<T>::grow_() {
    reallocate_(capacity_ == 0 ? 1 : capacity_ * 2);
}

template <class T>
void Vector<T>::resize(std::size_t n) {
    if (n > capacity_) {
        std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_;
        while (new_capacity < n) {
            new_capacity *= 2;
        }
        reallocate_(new_capacity);
    }

    for (std::size_t i = size_; i < n; ++i) {
        data_[i] = T{};
    }
    size_ = n;
}

template <class T>
void Vector<T>::push_back(T value) {
    if (size_ == capacity_) {
        grow_();
    }
    data_[size_] = value;
    ++size_;
}

template <class T>
void Vector<T>::reserve(std::size_t n) {
    if (n > capacity_) {
        reallocate_(n);
    }
}

template <class T>
void Vector<T>::shrink_to_fit() {
    if (size_ < capacity_) {
        reallocate_(size_);
    }
}

template <class T>
void Vector<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("Error: Vector is empty!");
    }
    --size_;
}

template <class T>
void Vector<T>::clear() {
    size_ = 0;
}
