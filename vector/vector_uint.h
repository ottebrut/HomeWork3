#ifndef VECTOR_UINT_H
#define VECTOR_UINT_H

#include <memory>
#include <vector>

#define uint unsigned int

class vector_uint {
public:
    vector_uint();
    ~vector_uint();

    uint size() const;

    void pop_back();
    void push_back(uint v);
    void resize(uint new_size);
    void clear();

    uint operator[](size_t ind) const;
    uint &operator[](size_t ind);

    vector_uint &operator=(vector_uint const &other);

private:
    std::shared_ptr<std::vector<uint>> big;
    uint small;
    size_t _size;

    bool is_big() const;
    void check();
};

#endif //BIGINT_vector_uint_H
