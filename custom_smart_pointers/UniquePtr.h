template <typename T>
class UniquePtr{
    explicit UniquePtr(T* ptr = nullptr);

    ~UniquePtr();

    // Copy Constructor 
    // = delete; tells compiler that we aren't allowed to call it
    // if we copy for example b = a; both b and a are pointing to the same object
    // and when they are destroyed we are calling delete twice which is undefined behaviour

    UniquePtr(const UniquePtr&) = delete; // UniquePtr<int> b = a; NOT ALLOWED
    UniquePtr& operator=(const UniquePtr&) = delete; // UniquePtr<int> b;
                                                    // b = a; NOT ALLOWED

    // Move Constructor
    // This is how we transfer ownership, instead of b = a; we do b = std::move(a);
    // this way b.ptr_ will point to what a.ptr_ was pointing at and a.ptr_ = nullptr;
    // OWNERSHIP IS TRANSFERED not DUPLICATED
    //&& -> references an object whos resources may be moved from.
    UniquePtr(UniquePtr&& other) noexcept;
    UniquePtr& operator=(UniquePtr&& other) noexcept;

    // return derefenced value
    T& operator*() const;

    // return raw pointer
    // use the object your pointer points to
    T* operator->() const;

    // return raw pointer
    // give me the raw pointer being stored
    T* get() const;


    T* release(); //gives up ownership WITHOUT delting the object
    void reset(T* ptr = nullptr); //delete the current owned object and optionalyl starts owning a new one

    explicit operator bool() const; //returns a bool to check if the current uniqueptr points to something
private:
    T* ptr_;
};

#include "UniquePtr.cpp"
