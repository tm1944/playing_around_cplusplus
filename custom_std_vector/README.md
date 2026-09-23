# Custom vector

This project is a small C++20 implementation of a vector-like container. It is a learning project, not a drop-in replacement for `std::vector`.

The current version covers:

- class templates
- const-correct element access
- the Rule of Five
- RAII
- raw allocation with `std::allocator`
- explicit construction and destruction of elements
- size and capacity management

It does not yet cover the full `std::vector` contract. Missing work includes allocator propagation, strong exception guarantees, iterators, insertion and erasure, perfect forwarding, and the standard iterator invalidation rules.

## Build and run the tests

The code uses `std::construct_at`, so it requires C++20.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

You can also compile the test file directly:

```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic -I. tests/VectorTests.cpp -o vector_tests
./vector_tests
```

## Example

```cpp
#include "Vector.h"

int main() {
    Vector<int> values{2};
    values.push_back(10);
    values.push_back(20);
    values.push_back(30); // capacity grows when the allocation is full

    values[0] = 5;           // unchecked access
    int last = values.at(2); // checked access
}
```

The constructor argument is the initial capacity, not the initial size. `Vector<int>{4}` allocates room for four integers but contains no live elements until `push_back` or `resize` constructs them.

## How to read the implementation

Start with the three data members at the bottom of `Vector.h`:

```cpp
std::size_t capacity_{};
T* data_ = nullptr;
std::size_t size_{};
```

`data_` points to raw storage. `capacity_` says how many `T` objects can fit in that storage. `size_` says how many `T` objects are alive. The main invariant is:

```text
0 <= size_ <= capacity_
```

Only the range `[data_, data_ + size_)` contains constructed objects. The remaining capacity is storage, not a collection of default-constructed values.

Read the functions in this order:

1. Read the constructor and destructor. The constructor allocates storage. The destructor destroys each live element, then releases the storage.
2. Read `push_back`, `pop_back`, and `clear`. These show how `size_` tracks the live objects.
3. Read `grow_` and `reallocate_`. Reallocation allocates a larger block, copies the live elements into it, destroys the old elements, and releases the old block.
4. Read `reserve`, `resize`, and `shrink_to_fit`. Compare which operations change capacity, size, or both.
5. Read the copy operations. They create independent storage and copy every live element.
6. Read the move operations. They transfer the pointer, size, and capacity, then reset the source to an empty state.
7. Finish with `at`, `operator[]`, `front`, and `back`. These functions do not manage ownership. They expose elements already owned by the vector.

The tests in `tests/VectorTests.cpp` follow the same ideas through the public API. `test_object_lifetime` is useful when reading the raw-storage code because its counter checks that every constructed object is destroyed exactly once.

## Current design notes

`Vector.cpp` is included at the bottom of `Vector.h`. Template definitions must be visible when the compiler instantiates a template, so `Vector.cpp` acts as an implementation include rather than a separately compiled source file.

`at`, `front`, `back`, and `pop_back` throw `std::out_of_range` for invalid access. `operator[]` performs no bounds check, like `std::vector::operator[]`.

Growth currently doubles capacity. An empty vector grows from zero to one element of capacity.
