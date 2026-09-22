#include <iostream>

template <class T>
class Vector
{
public:
  Vector(std::size_t capacity);                 // Constructor
  Vector(const Vector<T> &other);               // Copy Constructor
  Vector<T> &operator=(const Vector<T> &other); // does a = b;
  ~Vector();                                    // Destructor
  std::size_t size() const;                     // returns a number of elements
  bool empty() const;                           // checks if std::vector is empty
  std::size_t get_capacity() const;             // returns capacity
  T front() const;                              // returns element at index 0
  T back() const;                               // returns last element
  void reserve(std::size_t n);                  // reserves storage
  void push_back(T value);                      // adds element to array, reallocates memory if necessary
  void pop_back();                              // removes last element from array
  void clear();                                 // clears the array and frees memory
  void shrink_to_fit();                         // shrinks array capacity to size
  void resize(std::size_t n);                   // resizes array + memory to size n

private:
  std::size_t capacity_{};
  T *data_ = nullptr;
  std::size_t size_{};

  void copy_new_array_(T *new_data);
  void allocate_new_array(std::size_t capacity);
};

#include "Vector.cpp"
