#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <vector>
#include "vector/vector_uint.h"

#define ull unsigned long long
#define uint unsigned int

using namespace std;

class big_integer {
public:
        big_integer();
        big_integer(big_integer const& obj);
        big_integer(int n);
        explicit big_integer(string const& s);
        ~big_integer();

        big_integer& operator=(big_integer const& other);

        big_integer& operator+=(big_integer const& rhs);
        big_integer& operator-=(big_integer const& rhs);
        big_integer& operator*=(big_integer const& rhs);
        big_integer& operator/=(big_integer const& rhs);
        big_integer& operator%=(big_integer const& rhs);

        big_integer& operator&=(big_integer const& rhs);
        big_integer& operator|=(big_integer const& rhs);
        big_integer& operator^=(big_integer const& rhs);

        big_integer& operator<<=(int rhs);
        big_integer& operator>>=(int rhs);

        big_integer operator+() const;
        big_integer operator-() const;
        big_integer operator~() const;

        big_integer& operator++();
        big_integer operator++(int);

        big_integer& operator--();
        big_integer operator--(int);

        friend bool operator==(big_integer const& a, big_integer const& b);
        friend bool operator!=(big_integer const& a, big_integer const& b);
        friend bool operator<(big_integer const& a, big_integer const& b);
        friend bool operator>(big_integer const& a, big_integer const& b);
        friend bool operator<=(big_integer const& a, big_integer const& b);
        friend bool operator>=(big_integer const& a, big_integer const& b);

        friend string to_string(big_integer const& a);
private:
        vector_uint number;
        bool sign;
        big_integer(vector_uint const& obj, bool sign);
        void check_sign();

        void swap(big_integer &a);
        friend int compare(const vector_uint &a, const vector_uint &b);
        friend int compare(big_integer const& a, big_integer const& b, bool abs);
        friend bool compare_equal_vector(const vector_uint &a, const vector_uint &b);
        void subtract_equal_vector(vector_uint &a, const vector_uint &b);

        void add(vector_uint &res, vector_uint const &a, vector_uint const &b);
        void subtract(vector_uint &res, vector_uint const &a, vector_uint const &b);
        void multiply(vector_uint &res, vector_uint const &a, vector_uint const &b);
        void divide(vector_uint &res, vector_uint const &a, vector_uint const &b);

        void long_mul_short(vector_uint &res, vector_uint const &a, const unsigned int b);

        void long_and(vector_uint &res, vector_uint const &a, vector_uint const &b);
        void long_or(vector_uint &res, vector_uint const &a, vector_uint const &b);
        void long_xor(vector_uint &res, vector_uint const &a, vector_uint const &b);
        void long_not(vector_uint &res, vector_uint const &a);
        void right_shift(vector_uint &res, vector_uint const &a, const int b);
        void left_shift(vector_uint &res, vector_uint const &a, const int b);

        vector_uint big_integer_to_byte_vector(big_integer const &a);
        big_integer byte_vector_to_big_integer(vector_uint const &a);

        void long_div_short(vector_uint &res, vector_uint const &a, const uint b);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

template<typename T>
void clear_end(vector<T> &a);
template<typename T>
ull mod_division(vector<T> &a, ull module_base, ull input_base);

void clear_end(vector_uint &a);
ull mod_division(vector_uint &a, ull module_base, ull input_base);

#endif
