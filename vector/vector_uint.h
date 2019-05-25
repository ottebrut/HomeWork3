#ifndef VECTOR_UINT_H
#define VECTOR_UINT_H

#include <string.h>
#include <memory>

#define uint unsigned int

using namespace std;

class vector_uint {
public:
	vector_uint();
	~vector_uint();
	vector_uint(size_t size);
	vector_uint(const vector_uint &other);

	vector_uint& operator=(const vector_uint &other);

	size_t size() const;

	void pop_back();
	void push_back(const uint32_t x);
	uint back();
	uint* begin();
	uint const* begin() const;
	uint* end();
	uint const* end() const;

	void reserve(size_t new_capacity);
	void resize(size_t size);
	void clear();

	uint& operator[](size_t index);
	uint const& operator[](size_t index) const;

	void change_ptr();
private:
	const static int SMALL_SIZE = 4;
	
	size_t size_;
	size_t capacity_;

	struct big_object {
		big_object(size_t new_capacity, uint* ptr);
		size_t capacity;
		shared_ptr<uint> ptr;
		void swap(big_object &other);
	};

	union data {
		uint small_obj[SMALL_SIZE];
		big_object big_obj;
		data();
		~data();
	} data_;

	struct my_deleter {
		void operator()(uint* ptr) {
			operator delete(ptr);
		}
	};

	friend uint* copy_data(size_t capacity, size_t size, const uint *src);

	uint* my_data;

	void swap(vector_uint &other);
	void swap_big_and_small(data &big, data &small);
	void set_capacity(size_t new_capacity);
	size_t increase_capacity(size_t new_size) const;
	uint get_capacity() const;
	void switch_to_big(size_t new_capacity);
	bool is_big() const;
};

#endif
