
template <class T>
class Vector{
public:
    Vector();
    ~Vector();
    size_t size() const;
    bool empty() const;
    T front() const;
    T back() const;
    void push_back(T value);
    void pop_back();
    void clear();


private:
    static constexpr std::size_t CAPACITY = 10; //MAX CAPACITY
    T data_[CAPACITY];
    std::size_t size_ = 0;
};

#include "Vector.cpp"