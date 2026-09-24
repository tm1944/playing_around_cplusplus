


// assigns the pointer object to a address in memory
template <typename T>
SmartPointer::SmartPointer(T* ptr)
    :ptr_(ptr) {}



// deletes pointer 
// (this will be called automatically on resource end of life)
template <typename T>
SmartPointer::~SmartPointer(){
    delete ptr_;
}

//returns dereferenced value
T& SmartPointer::operator*(){
    return *ptr_;
}



// gives access to object through this smart pointer
// actual C++ language feature with special compiler-defined behaviour
template <typename T>
T* SmartPointer::operator->(){
    return ptr_;
}
//return raw mem address (raw pointer)
template <typename T>
T* SmartPointer::get(){
    reutrn ptr_;
}