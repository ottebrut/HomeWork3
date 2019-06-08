#include "big_integer.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

const static ull MAX_UINT64 = -1ull;
const static uint MAX_UINT32 = ((ull)1 << 32) - 1;
const static int MAX_INT32 = (1u << 31) - 1;
const static ull BASE = (1ull << 32);

//CONSTRUCTORS

big_integer::big_integer() {
    number.push_back(0);
    sign = 0;
}

big_integer::big_integer(int n) {
    if (n >= 0) {
        number.push_back((uint)n);
        sign = 0;
    } else {
        number.push_back(-(uint)n);
        sign = 1;
    }
}

big_integer::big_integer(std::string const& s) {
    if (s.empty()) {
        big_integer();
        return;
    }
    size_t pos = 0;
    if (s[0] == '-') {
        sign = 1;
        pos++;
    } else {
        sign = 0;
    }
    vector<int> tmp;
    for(size_t i = pos; i < s.size(); ++i) {
        tmp.push_back((int)(s[i] - '0'));
    }
    std::reverse(tmp.begin(), tmp.end());
    while((tmp.size() != 1 || tmp[0] != 0) && tmp.size() != 0) {
        uint mod = (uint)mod_division(tmp, BASE, 10);
        number.push_back(mod);
    }
    clear_end(number);
}

big_integer::big_integer(big_integer const& obj) {
    number = obj.number;
    sign = obj.sign;
}

big_integer::big_integer(vector_uint const& obj, bool sign) {
    number = obj;
    this->sign = sign;
    check_sign();
}

big_integer::~big_integer() {
}

template<typename T>
ull mod_division(vector<T> &a, ull module_base, ull input_base) {
    ull cur = 0;
    for(size_t i = a.size() - 1; i < MAX_UINT64; i--) {
        cur = cur * input_base + a[i];
        a[i] = cur / module_base;
        cur %= module_base;
    }
    clear_end(a);
    return cur;
}

ull mod_division(vector_uint &a, ull module_base, ull input_base) {
    ull cur = 0;
    for(size_t i = a.size() - 1; i < MAX_UINT64; i--) {
        cur = cur * input_base + a[i];
        a[i] = cur / module_base;
        cur %= module_base;
    }
    clear_end(a);
    return cur;
}

template<typename T>
void clear_end(vector<T> &a) {
    if (a.size() == 0) {
        a.push_back(0);
        return;
    }
    size_t pos = a.size() - 1;
    while(pos < MAX_UINT64 && a[pos] == 0) {
        --pos;
    }
    a.resize(pos + 1);
    if (a.size() == 0) {
        a.push_back(0);
    }
}

void clear_end(vector_uint &a) {
    if (a.size() == 0) {
        a.push_back(0);
        return;
    }
    size_t pos = a.size() - 1;
    while(pos < MAX_UINT64 && a[pos] == 0) {
        --pos;
    }
    a.resize(pos + 1);
    if (a.size() == 0) {
        a.push_back(0);
    }
}

void big_integer::check_sign() {
    if (number.size() == 1 && number[0] == 0) {
        sign = 0;
    }
}

//TO STRING

std::string to_string(big_integer const &a) {
    if (a.number.size() == 0) {
        return "0";
    }
    vector_uint tmp(a.number);
    string str;
    while(tmp.size() != 1 || tmp[0] != 0) {
        uint mod = (char)mod_division(tmp, 10, BASE);
        str.push_back((char)(mod + '0'));
    }
    while(!str.empty() && str.back() == '0') {
        str.pop_back();
    }
    if (str.empty()) {
        return "0";
    }
    if (a.sign) {
        str.push_back('-');
    }
    reverse(str.begin(), str.end());
    return str;
}

//COMPARISON

int compare(const vector_uint &a, const vector_uint &b) {
    if (a.size() != b.size()) {
        return (int)a.size() - b.size();
    }
    for(size_t i = a.size() - 1; i < MAX_UINT64; --i) {
        if (a[i] != b[i]) {
            return a[i] < b[i] ? -1 : 1;
        }
    }
    return 0;
}

int compare(big_integer const& a, big_integer const& b, bool abs = false) {
    if (abs == 0 && a.sign != b.sign) {
        return a.sign - b.sign;
    }
    return compare(a.number, b.number);
}

bool operator<(big_integer const& a, big_integer const& b){
    return compare(a, b) < 0;
}

bool operator<=(big_integer const& a, big_integer const& b){
    return compare(a, b) <= 0;
}

bool operator>(big_integer const& a, big_integer const& b){
    return compare(a, b) > 0;
}

bool operator>=(big_integer const& a, big_integer const& b){
    return compare(a, b) >= 0;
}

bool operator==(big_integer const& a, big_integer const& b){
    return compare(a, b) == 0;
}

bool operator!=(big_integer const& a, big_integer const& b){
    return compare(a, b) != 0;
}

//ASSINGMENT WITH OPERATIONS

void big_integer::swap(big_integer &a) {
    std::swap(this->number, a.number);
    std::swap(this->sign, a.sign);
}

big_integer& big_integer::operator=(big_integer const&a) {
    big_integer cur(a);
    swap(cur);
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    if (sign == rhs.sign) {
        add(number, number, rhs.number);
        return *this;
    }
    int comp = compare(*this, rhs, true);
    if (comp == 0) {
        number.resize(1);
        number[0] = 0;
        sign = 0;
        return *this;
    }
    if (comp > 0) {
        subtract(number, number, rhs.number);
        return *this;
    }
    subtract(number, rhs.number, number);
    sign = rhs.sign;
    check_sign();
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    return *this += -rhs;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    sign ^= rhs.sign;
    big_integer copy = *this;
    multiply(number, copy.number, rhs.number);
    check_sign();
    return *this;
}


big_integer& big_integer::operator/=(big_integer const& rhs) {
    sign ^= rhs.sign;
    big_integer copy = *this;
    divide(number, copy.number, rhs.number);
    check_sign();
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
    vector_uint quotient;
    divide(quotient, number, rhs.number);
    vector_uint mul;
    multiply(mul, quotient, rhs.number);
    subtract(number, number, mul);
    check_sign();
    return *this;
}

void big_integer::add(vector_uint &res, const vector_uint &a, const vector_uint &b) {
    size_t len = std::max(a.size(), b.size()) + 1;
    size_t a_size = a.size(), b_size = b.size();
    res.resize(len);
    ull carry = 0;
    for (size_t i = 0; i < len; ++i) {
        ull tmp = (ull)(i < a_size ? a[i] : 0) + (i < b_size ? b[i] : 0) + carry;
        res[i] = tmp % BASE;
        carry = tmp / BASE;
    }
    clear_end(res);
}

void big_integer::subtract(vector_uint &res, const vector_uint &a, const vector_uint &b) {
    size_t len = std::max(a.size(), b.size());
    size_t b_size = b.size();
    res.resize(len);
    ull carry = 0;
    for (size_t i = 0; i < b_size || carry; ++i) {
        ull tmp = carry + (i < b_size ? b[i] : 0);
        if (a[i] < tmp) {
            res[i] = BASE - tmp + a[i];
            carry = 1;
        } else {
            res[i] = a[i] - tmp;
            carry = 0;
        }
    }
    clear_end(res);
}

void big_integer::multiply(vector_uint &res, const vector_uint &a, const vector_uint &b) {
    size_t len = a.size() + b.size();
    res.clear();
    res.resize(len);
    for (size_t i = 0; i < a.size(); ++i) {
        ull carry = 0;
        for(size_t j = 0; j < b.size() || carry; ++j) {
            ull tmp = (ull)res[i + j] + (ull)a[i] * (j < b.size() ? b[j] : 0) + carry;
            res[i + j] = tmp % BASE;
            carry = tmp / BASE;
        }
    }
    clear_end(res);
}

        //division

void big_integer::long_mul_short(vector_uint &res, const vector_uint &a, const uint b) {
    size_t a_size = a.size();
    res.resize(a.size() + 1);
    uint carry = 0;
    for(size_t i = 0; i < a_size; ++i) {
        ull tmp = (ull)a[i] * b + carry;
        res[i] = tmp % BASE;
        carry = tmp / BASE;
    }
    res[a_size] = carry;
}

bool compare_equal_vector(const vector_uint &a, const vector_uint &b) {
    for(size_t i = a.size() - 1; i < MAX_UINT64; --i) {
        if (a[i] != b[i]) {
            return a[i] < b[i];
        }
    }
    return 0;
}

void big_integer::subtract_equal_vector(vector_uint &a, const vector_uint &b) {
    ull carry = 0;
    for(size_t i = 0; i < b.size() || carry; ++i) {
        ull tmp = (ull)carry + (i < b.size() ? b[i] : 0);
        if (a[i] < tmp) {
            a[i] = BASE - tmp + a[i];
            carry = 1;
        } else {
            a[i] -= tmp;
            carry = 0;
        }
    }
}

void big_integer::long_div_short(vector_uint &res, const vector_uint &a, const uint b) {
    size_t a_size = a.size();
    res.resize(a.size());
    uint carry = 0;
    for(size_t i = a_size - 1; i < UINT64_MAX; i--)
    {
        ull tmp = (ull)carry * BASE + a[i];
        res[i] = tmp / b;
        carry = tmp % b;
    }
    clear_end(res);
}

void big_integer::divide(vector_uint &res, vector_uint const &a, vector_uint const &b) {
    int sign = compare(a, b);
    res.clear();
    if (sign < 0) {
        res.push_back(0);
        return;
    }
    if (sign == 0) {
        res.push_back(1);
        return;
    }
    if(b.size() == 1)
    {
        long_div_short(res, a, b[0]);
        return;
    }
    uint d = (uint)(BASE / (b[b.size() - 1] + 1));
    vector_uint u, v;
    u.resize(a.size()), v.resize(b.size());
    for(size_t i = 0; i < a.size(); i++) {
        u[i] = a[i];
    }
    for(size_t i = 0; i < b.size(); i++) {
        v[i] = b[i];
    }
    long_mul_short(u, a, d);
    clear_end(u);
    long_mul_short(v, b, d);
    clear_end(v);
    size_t n = u.size(), m = v.size(), len = n - m + 1;
    res.resize(len);
    vector_uint dividend, divider;
    dividend.resize(m + 1), divider.resize(m + 1);
    for(size_t i = 0; i < m; ++i) {
        dividend[i] = u[n + i - m];
    }
    dividend[m] = n < u.size() ? u[n] : 0;
    for(size_t i = 0; i < len; ++i) {
        dividend[0] = u[n - m - i];
        uint tmp = std::min((BASE * dividend[m] + dividend[m - 1]) / v[v.size() - 1], BASE - 1);
        long_mul_short(divider, v, tmp);
        while(compare_equal_vector(dividend, divider)) {
            long_mul_short(divider, v, --tmp);
        }
        res[len - 1 - i] = tmp;
        subtract_equal_vector(dividend, divider);
        for(size_t j = m; j > 0; --j) {
            dividend[j] = dividend[j - 1];
        }
    }
    clear_end(res);
}

        //end division

big_integer& big_integer::operator&=(big_integer const &rhs) {
    long_and(number, big_integer_to_byte_vector(*this), big_integer_to_byte_vector(rhs));
    return *this = byte_vector_to_big_integer(number);
}

big_integer& big_integer::operator|=(big_integer const &rhs) {
    long_or(number, big_integer_to_byte_vector(*this), big_integer_to_byte_vector(rhs));
    *this = byte_vector_to_big_integer(number);
    return *this;
}

big_integer& big_integer::operator^=(big_integer const &rhs) {
    long_xor(number, big_integer_to_byte_vector(*this), big_integer_to_byte_vector(rhs));
    return *this = byte_vector_to_big_integer(number);
}

big_integer& big_integer::operator<<=(int rhs) {
    left_shift(number, big_integer_to_byte_vector(*this),  rhs);
    return *this = byte_vector_to_big_integer(number);
}

big_integer& big_integer::operator>>=(int rhs) {
    right_shift(number, big_integer_to_byte_vector(*this),  rhs);
    return *this = byte_vector_to_big_integer(number);
}

void big_integer::long_and(vector_uint &res, vector_uint const &a, vector_uint const &b) {
    size_t len = std::max(a.size(), b.size());
    size_t a_size = a.size(), b_size = b.size();
    res.resize(len);
    for(size_t i = 0; i < len; i++) {
        res[i] = a[std::min(a_size - 1, i)] & b[std::min(b_size - 1, i)];
    }
}

void big_integer::long_or(vector_uint &res, vector_uint const &a, vector_uint const &b) {
    size_t len = std::max(a.size(), b.size());
    size_t a_size = a.size(), b_size = b.size();
    res.resize(len);
    for(size_t i = 0; i < len; i++) {
        res[i] = a[std::min(a_size - 1, i)] | b[std::min(b_size - 1, i)];
    }
}

void big_integer::long_xor(vector_uint &res, vector_uint const &a, vector_uint const &b) {
    size_t len = std::max(a.size(), b.size());
    size_t a_size = a.size(), b_size = b.size();
    res.resize(len);
    for(size_t i = 0; i < len; i++) {
        res[i] = a[std::min(a_size - 1, i)] ^ b[std::min(b_size - 1, i)];
    }
}

void big_integer::long_not(vector_uint &res, vector_uint const &a) {
    size_t len = a.size();
    res.resize(len);
    for(size_t i = 0; i < len; i++) {
        res[i] = ~res[i];
    }
}

void big_integer::right_shift(vector_uint &res, vector_uint const &a, const int b) {
    uint shift_in = b % 32;
    uint shift_out = b / 32;
    vector_uint tmp;
    ull cur = 0;
    uint sign = a[a.size() - 1];
    cur = a[shift_out] >> shift_in;
    for(size_t i = shift_out; i < a.size() - 1; i++) {
        ull buf = (ull)a[i + 1] << (32 - shift_in);
        cur |= buf;
        tmp.push_back(cur & (BASE - 1));
        cur >>= 32;
    }
    tmp.push_back(sign);
    res = tmp;
}

void big_integer::left_shift(vector_uint &res, vector_uint const &a, const int b) {
    uint shift_in = b % 32;
    uint shift_out = b / 32;
    vector_uint tmp;
    tmp.resize(shift_out);
    ull cur = 0;
    uint sign = a[a.size() - 1];
    for(size_t i = 0; i < a.size(); i++) {
        cur |= ((ull)a[i] << shift_in);
        tmp.push_back(cur & (BASE - 1));
        cur >>= 32;
    }
    tmp.push_back(sign);
    res = tmp;
}

vector_uint big_integer::big_integer_to_byte_vector(const big_integer &a) {
    vector_uint res(a.number);
    res.push_back(0);
    if(a.sign == 0) {
        return res;
    }
    for(size_t i = 0; i < res.size(); i++) {
        res[i] = ~res[i];
    }
    vector_uint one;
    one.push_back(1);
    add(res, res, one);
    return res;
}

big_integer big_integer::byte_vector_to_big_integer(const vector_uint &a) {
    vector_uint res(a);
    int sign = 0;
    if(a[a.size() - 1] == MAX_UINT32)
    {
        res.pop_back();
        sign = 1;
        long_not(res, res);
        vector_uint one;
        one.push_back(1);
        add(res, res, one);
    }
    clear_end(res);
    return big_integer(res, sign);
}

//UNAR OPERATIONS

big_integer& big_integer::operator++() {
    const big_integer one(1);
    return *this += one;
}

big_integer& big_integer::operator--() {
    const big_integer one(1);
    return *this -= one;
}

big_integer big_integer::operator++(int) {
    big_integer temp(*this);
    ++(*this);
    return temp;
}

big_integer big_integer::operator--(int) {
    big_integer temp(*this);
    --(*this);
    return temp;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    return big_integer(number, !sign);
}

big_integer big_integer::operator~() const {
    const big_integer one(1);
    return -*this - one;
}

//OPERATIONS

big_integer operator+(big_integer a, big_integer const& b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, const int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, const int b) {
    return a >>= b;
}
