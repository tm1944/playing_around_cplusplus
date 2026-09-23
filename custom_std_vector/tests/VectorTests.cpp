#include "Vector.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

int failures = 0;

void check(bool condition, const char* expression, int line) {
    if (!condition) {
        std::cerr << "line " << line << ": check failed: " << expression << '\n';
        ++failures;
    }
}

#define CHECK(expression) check(static_cast<bool>(expression), #expression, __LINE__)

template <class Exception, class Function>
void check_throws(Function function, const char* expression, int line) {
    try {
        function();
    } catch (const Exception&) {
        return;
    } catch (...) {
        std::cerr << "line " << line << ": wrong exception from: " << expression << '\n';
        ++failures;
        return;
    }

    std::cerr << "line " << line << ": expected exception from: " << expression << '\n';
    ++failures;
}

#define CHECK_THROWS_AS(expression, exception_type) \
    check_throws<exception_type>([&] { static_cast<void>(expression); }, #expression, __LINE__)

struct Tracked {
    static inline int alive = 0;

    int value = 0;

    Tracked() { ++alive; }
    explicit Tracked(int value) : value(value) { ++alive; }
    Tracked(const Tracked& other) : value(other.value) { ++alive; }
    ~Tracked() { --alive; }
};

void test_empty_vector() {
    Vector<int> values{3};

    CHECK(values.empty());
    CHECK(values.size() == 0);
    CHECK(values.get_capacity() == 3);
    CHECK_THROWS_AS(values.front(), std::out_of_range);
    CHECK_THROWS_AS(values.back(), std::out_of_range);
    CHECK_THROWS_AS(values.pop_back(), std::out_of_range);
}

void test_push_and_element_access() {
    Vector<std::string> values{2};
    values.push_back("red");
    values.push_back("blue");

    CHECK(values.size() == 2);
    CHECK(values.front() == "red");
    CHECK(values.back() == "blue");
    CHECK(values.at(1) == "blue");

    values[1] = "green";
    CHECK(values.at(1) == "green");
    CHECK_THROWS_AS(values.at(2), std::out_of_range);

    const Vector<std::string>& read_only = values;
    CHECK(read_only[0] == "red");
    CHECK(read_only.at(1) == "green");
}

void test_growth_and_reserve() {
    Vector<int> values{0};
    values.push_back(10);
    values.push_back(20);
    values.push_back(30);

    CHECK(values.size() == 3);
    CHECK(values.get_capacity() == 4);
    CHECK(values[0] == 10);
    CHECK(values[1] == 20);
    CHECK(values[2] == 30);

    values.reserve(10);
    CHECK(values.size() == 3);
    CHECK(values.get_capacity() == 10);
    CHECK(values[2] == 30);

    values.reserve(5);
    CHECK(values.get_capacity() == 10);
}

void test_resize_and_shrink_to_fit() {
    Vector<int> values{1};
    values.push_back(7);
    values.resize(4);

    CHECK(values.size() == 4);
    CHECK(values.get_capacity() == 4);
    CHECK(values[0] == 7);
    CHECK(values[1] == 0);
    CHECK(values[2] == 0);
    CHECK(values[3] == 0);

    values.resize(2);
    CHECK(values.size() == 2);
    CHECK(values.get_capacity() == 4);

    values.shrink_to_fit();
    CHECK(values.size() == 2);
    CHECK(values.get_capacity() == 2);
    CHECK(values[0] == 7);
}

void test_pop_and_clear() {
    Vector<int> values{4};
    values.push_back(1);
    values.push_back(2);
    values.pop_back();

    CHECK(values.size() == 1);
    CHECK(values.back() == 1);

    values.clear();
    CHECK(values.empty());
    CHECK(values.get_capacity() == 4);
}

void test_copy_is_deep() {
    Vector<int> original{4};
    original.push_back(1);
    original.push_back(2);

    Vector<int> copied{original};
    copied[0] = 99;

    CHECK(original[0] == 1);
    CHECK(copied[0] == 99);
    CHECK(copied.size() == original.size());
    CHECK(copied.get_capacity() == original.get_capacity());

    Vector<int> assigned{1};
    assigned.push_back(8);
    assigned = original;
    assigned[1] = 77;

    CHECK(original[1] == 2);
    CHECK(assigned[1] == 77);

    assigned = assigned;
    CHECK(assigned.size() == 2);
    CHECK(assigned[0] == 1);
}

void test_move_transfers_storage() {
    Vector<int> source{2};
    source.push_back(4);
    source.push_back(5);

    Vector<int> moved{std::move(source)};
    CHECK(moved.size() == 2);
    CHECK(moved[0] == 4);
    CHECK(source.empty());
    CHECK(source.get_capacity() == 0);

    source.push_back(9);
    CHECK(source.size() == 1);
    CHECK(source[0] == 9);

    Vector<int> assigned{1};
    assigned.push_back(12);
    assigned = std::move(moved);
    CHECK(assigned.size() == 2);
    CHECK(assigned[1] == 5);
    CHECK(moved.empty());

    assigned = std::move(assigned);
    CHECK(assigned.size() == 2);
    CHECK(assigned[0] == 4);
}

void test_object_lifetime() {
    CHECK(Tracked::alive == 0);
    {
        Vector<Tracked> values{1};
        values.push_back(Tracked{1});
        values.resize(3);
        CHECK(Tracked::alive == 3);

        values.pop_back();
        CHECK(Tracked::alive == 2);

        values.clear();
        CHECK(Tracked::alive == 0);
    }
    CHECK(Tracked::alive == 0);
}

} // namespace

int main() {
    test_empty_vector();
    test_push_and_element_access();
    test_growth_and_reserve();
    test_resize_and_shrink_to_fit();
    test_pop_and_clear();
    test_copy_is_deep();
    test_move_transfers_storage();
    test_object_lifetime();

    if (failures != 0) {
        std::cerr << failures << " test check(s) failed\n";
        return 1;
    }

    std::cout << "All vector tests passed\n";
    return 0;
}
