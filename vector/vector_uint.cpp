#include <memory>
#include "vector_uint.h"

using namespace std;

vector_uint::vector_uint() {
    small = 0;
    _size = 0;
}

vector_uint::~vector_uint() {
    if (is_big()) {
        big.reset();
    }
}

uint vector_uint::size() const {
    return _size;
}

uint vector_uint::operator[](size_t ind) const {
    if (is_big()) {
        return big->operator[](ind);
    }
    return small;
}

uint &vector_uint::operator[](size_t ind) {
    if (is_big()) {
        check();
        return big->operator[](ind);
    }
    return small;
}

void vector_uint::pop_back() {
    if (_size > 2) {
        check();
        big->pop_back();
    } else if (_size == 2) {
        small = big->operator[](0);
        big.reset();
    } else {
        small = 0;
    }
    _size--;
}

void vector_uint::push_back(uint v) {
    if (is_big()) {
        check();
        big->push_back(v);
    } else if (_size > 0) {
        big = make_shared<vector<uint>>(1, small);
        big->push_back(v);
    } else {
        small = v;
    }
    _size++;
}

void vector_uint::resize(uint new_size) {
    if (is_big()) {
        if (new_size < 2) {
            small = operator[](0);
            big.reset();
        } else {
            check();
            big->resize(new_size);
        }
    } else if (new_size > 1) {
        big = make_shared<vector<uint>>(new_size, small);
    } else if (!new_size) {
        small = 0;
    }
    _size = new_size;
}

bool vector_uint::is_big() const {
    return _size > 1;
}

void vector_uint::check() {
    if (!big.unique()) {
        big = make_shared<vector<uint>>(*big);
    }
}

vector_uint &vector_uint::operator=(vector_uint const &other) {
    if (is_big()) {
        big.reset();
    }
    if (other.is_big()) {
        big = other.big;
    } else {
        small = other.small;
    }
    _size = other._size;
    return *this;
}

void vector_uint::clear() {
    if (is_big()) {
        big.reset();
    }
    small = 0;
    _size = 0;
}
