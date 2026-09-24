
template <typename T>
class SmartPointer{
    // RAII -> the lifetime of a resource is tied to the lifetime of an object
    explicit SmartPointer(T* ptr);

    ~SmartPointer(); //destructor

    T& operator*() const; //dereferencing

    T* operator->() const;

    T* get() const; //returns raw address

private:
    T* ptr_;
};


#include "SmartPointer.cpp";