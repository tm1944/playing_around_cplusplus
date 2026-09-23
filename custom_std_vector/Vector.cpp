#include <stdexcept>


// Allocates raw storage capable of holding capacity elements,without creating elements yet
template <class T>
Vector<T>::Vector(std::size_t capacity)
    : capacity_(capacity), data_(allocator_.allocate(capacity_)) {}




// Copy Constructor. Creates a new vector with its own storage and makes a deep-copu from other
template <class T>
Vector<T>::Vector(const Vector<T>& other)
    : capacity_(other.capacity_), data_(allocator_.allocate(other.capacity_)), size_(other.size_) {
    for (std::size_t i = 0; i < size_; ++i) {
        std::construct_at(data_+i,other.data_[i]);
    }
}

// Move Constructor. transfer ownership of another vector's storage to a newly created vector (leaves source empty)
template <class T>
Vector<T>::Vector(Vector<T>&& other) noexcept
    : capacity_(other.capacity_), data_(other.data_), size_(other.size_) {
        other.size_ = 0;
        other.data_ = nullptr;
        other.capacity_ = 0;
    }


// Copy assignment. Replaces an existing vector content with independent copies of another vectors elements
template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this == &other) {
        return *this;
    }
    T* new_data = allocator_.allocate(other.capacity_);
    for (std::size_t i = 0; i < other.size_; ++i) {
        std::construct_at(new_data + i,other.data_[i]);
    }
    for(std::size_t i = 0; i < size_;++i){
        std::destroy_at(data_+i);
    }
    if (data_ != nullptr){
        allocator_.deallocate(data_,capacity_);
    }
    data_ = new_data;
    capacity_ = other.capacity_;
    size_ = other.size_;
    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept{
    if (this == &other){
        return *this;
    }
    for(std::size_t it = 0; it < size_;++it){
        std::destroy_at(data_+it);
    }

    if (data_ != nullptr){
        allocator_.deallocate(data_,capacity_);
    }

    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
}

//destructor 
template <class T>
Vector<T>::~Vector() {
    for(std::size_t it = 0; it < size_; ++it){
        std::destroy_at(data_ + it);
    }
    if(data_ != nullptr){
        allocator_.deallocate(data_,capacity_);
    }
}


//end of constructors + destructor

//provides unchecks acces to an element. const overload prevents modification through a const vector
template <class T>
const T& Vector<T>::operator[](std::size_t index) const{
    
    return data_[index];
}

template <class T>
T& Vector<T>::operator[](std::size_t index){
    return data_[index];
}


// provides bound-checked elements acces and throws if the index is invalid. the const overload prevents modification
template <class T>
const T& Vector<T>::at(std::size_t index) const{
    if (index >= size_){
        throw std::out_of_range("Error: index is out of range.\n");
    }
    return data_[index];
}

template <class T>
T& Vector<T>::at(std::size_t index){
    if (index >= size_){
        throw std::out_of_range("Error: index is out of range.");
    }
    return data_[index];
}


// returns the number of live elements currently in the vector
template <class T>
std::size_t Vector<T>::size() const {
    return size_;
}

//returns how many elements can fit in the currently allocated storage
template <class T>
std::size_t Vector<T>::get_capacity() const {
    return capacity_;
}

//checks whether size_== 0
template <class T>
bool Vector<T>::empty() const {
    return size_ == 0;
}

// returns a const reference to the first element. (throws if empty)
template <class T>
const T& Vector<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Error: Vector is empty!");
    }
    return data_[0];
}

// returns a const refence to the last element. (throws if empty)
template <class T>
const T& Vector<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Error: Vector is empty!");
    }
    return data_[size_ - 1];
}

//allocates a new raw memory block, constructs the existing elements there, destroys the old elements and releases the old storage
template <class T>
void Vector<T>::reallocate_(std::size_t new_capacity) {
    T* new_data = allocator_.allocate(new_capacity);
    for (std::size_t i = 0; i < size_; ++i){
        std::construct_at(new_data + i,data_[i]);
    }
    for(std::size_t i = 0; i < size_;++i){
        std::destroy_at(data_ + i);
    }
    if(data_ != nullptr){
        allocator_.deallocate(data_,capacity_);
    }

    data_ = new_data;
    capacity_ = new_capacity;
    
}

// internal helper that increases capaciyt, currently doubling it or going from 0 to 1
template <class T>
void Vector<T>::grow_() {
    reallocate_(capacity_ == 0 ? 1 : capacity_ * 2);
}


// changes the number of live elements. Growing construct new elements & shrinking destroys elements
template <class T>
void Vector<T>::resize(std::size_t n) {
    if (n > capacity_) {
        std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_;
        while (new_capacity < n) {
            new_capacity *= 2;
        }
        reallocate_(new_capacity);
    }

    if(n > size_){
        for (std::size_t i = size_; i < n; ++i) {
            std::construct_at(data_ + i);
        }
    }
    else if(n < size_){
        for(size_t i = n;i < size_;++i){
            std::destroy_at(data_ + i);
        }
    }
    size_ = n;
    }


//constructs a new element at the end, growing the allocation first if necessary
template <class T>
void Vector<T>::push_back(T value) {
    if (size_ == capacity_) {
        grow_();
    }
    std::construct_at(data_ + size_,value);
    ++size_;
}

// ensures capacity is at least n without changing the vectors size
template <class T>
void Vector<T>::reserve(std::size_t n) {
    if (n > capacity_) {
        reallocate_(n);
    }
}

// educes allocated capacity to match the current size
template <class T>
void Vector<T>::shrink_to_fit() {
    if (size_ < capacity_) {
        reallocate_(size_);
    }
}


// destroys the last live elements and decrease size by one
template <class T>
void Vector<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("Error: Vector is empty!");
    }
    std::destroy_at(data_ + (size_ - 1));
    --size_;
}

// destroys every live element and sets size to zero without releasing the allocated storage 
template <class T>
void Vector<T>::clear() {
    for(std::size_t it  = 0; it < size_;++it){
        std::destroy_at(data_+it);
    }
    size_ = 0;
    
}
