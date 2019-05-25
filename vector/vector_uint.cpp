#include "vector_uint.h"
#include <iostream>
#include <assert.h>

using namespace std;

uint* copy_data(size_t capacity, size_t size, const uint *src) {
    uint* dst = static_cast<uint*>(operator new(capacity * sizeof(uint)));
    memcpy(dst, src, size * sizeof(uint));
    memset(dst + size, 0, (capacity - size) * sizeof(uint));
    return dst;
}

vector_uint::vector_uint(): size_(0), capacity_(SMALL_SIZE), my_data(data_.small_obj) {
    memset(my_data, 0, SMALL_SIZE * sizeof(uint));
}

vector_uint::vector_uint(size_t size): vector_uint() {
    reserve(size);
    size_ = size;
}

vector_uint::vector_uint(const vector_uint &other): size_(other.size()) {
    if (other.is_big()) {
        new(&data_.big_obj) big_object(other.data_.big_obj);
        my_data = other.data_.big_obj.ptr.get();
    } else {
        for (size_t i = 0; i < SMALL_SIZE; ++i) {
            data_.small_obj[i] = other.data_.small_obj[i];
        }
        my_data = data_.small_obj;
    }
}

vector_uint::~vector_uint() {
    if (is_big()) {
        data_.big_obj.~big_object();
    }
}

void vector_uint::resize(size_t new_size) {
    reserve(new_size);
    size_ = new_size;
}

void vector_uint::reserve(size_t new_capacity) {
    if (new_capacity <= get_capacity()) return;
    set_capacity(new_capacity);
}

void vector_uint::set_capacity(size_t new_capacity) {
    if (is_big()) {
        data_.big_obj.ptr.reset(copy_data(new_capacity, size_, my_data));
        data_.big_obj.capacity = new_capacity;
        my_data = data_.big_obj.ptr.get();
    } else {
        switch_to_big(new_capacity);
    }
}

void vector_uint::switch_to_big(size_t new_capacity) {
    new(&data_.big_obj) big_object(new_capacity, copy_data(new_capacity, size_, my_data));
    my_data = data_.big_obj.ptr.get();
}

size_t vector_uint::size() const {
    return size_;
}

uint& vector_uint::operator[](size_t ind) {
    change_ptr();
    return my_data[ind];
}

const uint& vector_uint::operator[](size_t ind) const {
    return my_data[ind];
}

void vector_uint::swap_big_and_small(data &big, data &small) {
    uint tmp[SMALL_SIZE] = {};
    for(size_t i = 0; i < SMALL_SIZE; ++i) {
        tmp[i] = small.small_obj[i];
    }
    new(&small.big_obj) big_object(big.big_obj);
    big.big_obj.~big_object();
    for(size_t i = 0; i < SMALL_SIZE; ++i) {
        big.small_obj[i] = tmp[i];
    }
}

void vector_uint::swap(vector_uint &other) {
    if (!is_big() && !other.is_big()) {
        for(size_t i = 0; i < SMALL_SIZE; ++i) {
            std::swap(data_.small_obj[i], other.data_.small_obj[i]);
        }
    } else if (is_big() && other.is_big()) {
        std::swap(data_.big_obj, other.data_.big_obj);
        my_data = data_.big_obj.ptr.get();
        other.my_data = other.data_.big_obj.ptr.get();
    } else if (is_big()) {
        swap_big_and_small(data_, other.data_);
        my_data = data_.small_obj;
        other.my_data = other.data_.big_obj.ptr.get();
    } else {
        swap_big_and_small(other.data_, data_);
        other.my_data = other.data_.small_obj;
        my_data = data_.big_obj.ptr.get();
    }
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

vector_uint& vector_uint::operator =(vector_uint const &other) {
    vector_uint temp(other);
    swap(temp);
    return *this;
}

void vector_uint::pop_back() {
    change_ptr();
    size_--;
    my_data[size_] = 0;
}

void vector_uint::push_back(const uint a) {
    if (get_capacity() <= size_) {
        reserve(increase_capacity(size_));
    }
    my_data[size_] = a;
    size_++;
}

uint vector_uint::back() {
    return my_data[size_ - 1];
}

size_t vector_uint::increase_capacity(size_t new_size) const {
    if (new_size == 0) return SMALL_SIZE;
    return new_size * 3 / 2;
}

uint* vector_uint::begin() {
    return my_data;
}

uint const* vector_uint::begin() const {
    return my_data;
}

uint* vector_uint::end() {
    return my_data + size_;
}

uint const* vector_uint::end() const {
    return my_data + size_;
}

void vector_uint::clear() {
    vector_uint().swap(*this);
}

uint vector_uint::get_capacity() const {
    if (is_big()) return data_.big_obj.capacity;
    return SMALL_SIZE;
}

vector_uint::big_object::big_object(size_t new_capacity, uint *ptr): capacity(new_capacity), ptr(ptr) {
}

void vector_uint::big_object::swap(big_object &other) {
    std::swap(capacity, other.capacity);
    std::swap(ptr, other.ptr);
}

vector_uint::data::data() {
}

vector_uint::data::~data() {
}

bool vector_uint::is_big() const {
    return my_data != data_.small_obj;
}

void vector_uint::change_ptr() {
    if (is_big() && !data_.big_obj.ptr.unique())
    {
        data_.big_obj.ptr.reset(copy_data(size(), size(), my_data));
        my_data = data_.big_obj.ptr.get();
    }
}
