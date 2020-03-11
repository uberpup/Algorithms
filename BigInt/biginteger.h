#ifndef BIGINT_BIGINTEGER_H
#define BIGINT_BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>

class BigInteger {
public:
    BigInteger();
    BigInteger(int value);
    BigInteger(const BigInteger& big_int);
    BigInteger(std::string str);
    BigInteger(bool is_positive, std::vector<int> digits);
    ~BigInteger() = default;
    BigInteger& operator=(const BigInteger& big_int);

    BigInteger& operator++(); //prefix
    const BigInteger operator++(int); //postfix
    BigInteger& operator--();
    const BigInteger operator--(int);

    friend BigInteger operator+(const BigInteger& lhv, const BigInteger& rhv);
    friend BigInteger operator-(const BigInteger& lhv, const BigInteger& rhv);
    friend BigInteger operator%(const BigInteger& lhv, const BigInteger& rhv);
    friend BigInteger operator*(const BigInteger& lhv, const BigInteger& rhv);
    friend BigInteger operator/(const BigInteger& lhv, const BigInteger& rhv);

    friend BigInteger& operator+=(BigInteger& lhv, const BigInteger& rhv);
    friend BigInteger& operator-=(BigInteger& lhv, const BigInteger& rhv);
    friend BigInteger& operator%=(BigInteger& lhv, const BigInteger& rhv);
    friend BigInteger& operator*=(BigInteger& lhv, const BigInteger& rhv);
    friend BigInteger& operator/=(BigInteger& lhv, const BigInteger& rhv);

    friend bool operator==(const BigInteger& lhv, const BigInteger& rhv);
    friend bool operator!=(const BigInteger& lhv, const BigInteger& rhv);
    friend bool operator<=(const BigInteger& lhv, const BigInteger& rhv);
    friend bool operator>=(const BigInteger& lhv, const BigInteger& rhv);
    friend bool operator<(const BigInteger& lhv, const BigInteger& rhv);
    friend bool operator>(const BigInteger& lhv, const BigInteger& rhv);

    friend std::ostream& operator<<(std::ostream& stream, const BigInteger& big_int);
    friend std::istream& operator>>(std::istream& stream, BigInteger& big_int);

    const BigInteger operator-();
    explicit operator bool() const;
    std::string toString() const;
    BigInteger Abs() const;

private:
    void Diff(BigInteger& lhv, const BigInteger& rhv);
    void StripZeros();
    void AddZeros(size_t count);
    size_t CountForwardZeros() const;
    int Compare(const BigInteger& rhv) const;
    bool is_positive;
    std::vector<int> digits;
};

BigInteger::BigInteger() : is_positive(true), digits(1) {}

BigInteger::BigInteger(int value) {
    is_positive = true;
    if (value < 0) {
        value *= -1;
        is_positive = false;
    } else if (value == 0) {
        digits.push_back(0);
        return;
    }
    while (value > 0) {    // Filling digits in reverse-order
        digits.push_back(value % 10);
        value /= 10;
    }
}

BigInteger::BigInteger(std::string str) {
    if (str.empty()) {
        is_positive = true;
        digits.resize(1);
        return;
    }

    size_t shift;
    if (str[0] == '-') {
        is_positive = false;
        shift = 1;
    } else {
        is_positive = true;
        shift = 0;
    }
    digits.resize(str.length() - shift);
    for (size_t i = 0; i < str.length(); ++i) {
        digits[digits.size() - i - 1] = str[i + shift] - 48;
    }
}

BigInteger::BigInteger(bool is_positive,
        std::vector<int> digits) : is_positive(is_positive), digits(std::move(digits)) {}

BigInteger::BigInteger(const BigInteger& big_int) : is_positive(big_int.is_positive),
                                                    digits(big_int.digits) {}

BigInteger& BigInteger::operator=(const BigInteger& big_int) {
    if (this == &big_int) {
        return *this;
    }

    digits = big_int.digits;
    is_positive = big_int.is_positive;
    return *this;
}

BigInteger::operator bool() const{
    return *this != 0;
}

std::string BigInteger::toString() const{
    std::string str;
    //std::cerr <<"{}";
    auto temp = *this;
    temp.StripZeros();
    if (!temp.is_positive) {
        str += '-';
    }
    //size_t forward_zeros = CountForwardZeros();
    for (ssize_t i = temp.digits.size() - 1; i >= 0; --i) {
        str += std::to_string(temp.digits[i]);
    }
    if (str.empty()) {
        str = "0";
    }
    return str;
}

void BigInteger::StripZeros() {
    while (digits.back() == 0 && digits.size() > 1) {
        digits.pop_back();
    }
    if (digits.back() == 0 && !is_positive) {
        is_positive = true;
    }
}

size_t BigInteger::CountForwardZeros() const {
    size_t result = 0;
    ssize_t i = digits.size() - 1;
    while (digits[i] == 0 && i >= 0) {
        ++result;
        --i;
    }
    return result;
}

BigInteger& BigInteger::operator++() {
    *this += 1;
    return *this;
}

const BigInteger BigInteger::operator++(int) {
    BigInteger result;
    result = *this;
    ++(*this);
    return result;
}


BigInteger& BigInteger::operator--() {
    *this -= 1;
    return *this;
}

const BigInteger BigInteger::operator--(int) {
    BigInteger result;
    result = *this;
    --(*this);
    return result;
}

BigInteger operator+(const BigInteger& lhv, const BigInteger& rhv) {
    BigInteger result;
    result = lhv;
    result += rhv;
    return result;
}

BigInteger operator-(const BigInteger& lhv, const BigInteger& rhv) {
    BigInteger result;
    result = lhv;
    result -= rhv;
    return result;
}

BigInteger operator%(const BigInteger& lhv, const BigInteger& rhv) {
    BigInteger result;
    BigInteger current;
    result.digits.resize(lhv.digits.size());

    for (ssize_t i = result.digits.size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), 0);
        current.StripZeros();
        current.digits[0] = lhv.digits[i];

        size_t left, right, value;
        left = 0;
        right = 9;
        value = 0;
        while (left <= right) {
            size_t mid = (left + right) / 2;
            BigInteger temp = mid * rhv.Abs();
            temp.StripZeros();
            current.StripZeros();
            if (current.Compare(temp) >= 0) {
                value = mid;
                left = value + 1;
            } else {
                right = mid - 1;
            }
        }
        result.digits[i] = value;
        current -= value * rhv.Abs();
    }
    current.StripZeros();
    current.is_positive = lhv.is_positive;
    return current;
}

void ExtendVec(std::vector<int>& vec, size_t len) {
    while (len & (len - 1)) {
        ++len;
    }
    vec.resize(len);
}

std::vector<int> KaratsubaMul(std::vector<int>& first, std::vector<int>& second) {
    size_t len = first.size();
    std::vector<int> result(len * 2);
    if (len < 128) {
        for (size_t i = 0; i < len; ++i) {
            for (size_t j = 0; j < len; ++j) {
                result[i + j] += first[i] * second[j];
            }
        }
        for (size_t i = 0; i < result.size() - 1; ++i) {
            result[i + 1] += result[i] / 10;
            result[i] %= 10;
        }
        result.back() %= 10;
        return result;
    }
    size_t sep = len / 2;
    std::vector<int> first_left = {first.begin(), first.begin() + sep};
    std::vector<int> first_right = {first.begin() + sep, first.end()};
    std::vector<int> second_left = {second.begin(), second.begin() + sep};
    std::vector<int> second_right = {second.begin() + sep, second.end()};

    std::vector<int> mul_left = KaratsubaMul(first_left, second_left);
    std::vector<int> mul_right = KaratsubaMul(first_right, second_right);

    std::vector<int> first_sum(sep);
    std::vector<int> second_sum(sep);

    for (size_t i = 0; i < sep; ++i) {
        first_sum[i] = first_left[i] + first_right[i];
        second_sum[i] = second_left[i] + second_right[i];
    }

    std::vector<int> mul = KaratsubaMul(first_sum, second_sum);

    for (size_t i = 0; i < len; ++i) {
        mul[i] -= (mul_right[i] + mul_left[i]);
    }
    for (size_t i = 0; i < len; ++i) {
        result[i] = mul_right[i];
    }
    for (size_t i = len; i < len * 2; ++i) {
        result[i] = mul_left[i - len];
    }
    for (size_t i = sep; i < len + sep; ++i) {
        result[i] = mul[i - sep];
    }
    return result;
}

BigInteger operator*(const BigInteger& lhv, const BigInteger& rhv) {
    BigInteger lhv_copy, rhv_copy;
    lhv_copy = lhv;
    rhv_copy = rhv;
    ExtendVec(lhv_copy.digits, std::max(lhv.digits.size(), rhv.digits.size()));
    ExtendVec(rhv_copy.digits, std::max(lhv.digits.size(), rhv.digits.size()));
    auto result_digits = KaratsubaMul(lhv_copy.digits, rhv_copy.digits);

    return {((lhv.is_positive && rhv.is_positive) ||
             (!lhv.is_positive && !rhv.is_positive)), result_digits};
}

BigInteger operator/(const BigInteger& lhv, const BigInteger& rhv) {
    BigInteger result;
    BigInteger current;
    result.digits.resize(lhv.digits.size());
    for (ssize_t i = result.digits.size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), 0);
        current.StripZeros();
        current.digits[0] = lhv.digits[i];

        size_t left, right, value;
        left = 0;
        right = 9;
        value = 0;
        while (left <= right) {
            size_t mid = (left + right) / 2;
            BigInteger temp = mid * rhv.Abs();
            temp.StripZeros();
            current.StripZeros();
            if (current.Compare(temp) >= 0) {
                value = mid;
                left = value + 1;
            } else {
                right = mid - 1;
            }
        }
        result.digits[i] = value;
        current -= value * rhv.Abs();
    }
    result.StripZeros();
    result.is_positive = lhv.is_positive == rhv.is_positive;
    return result;
}

void BigInteger::Diff(BigInteger& lhv, const BigInteger& rhv) {
    int carry = 0;
    for (size_t i = 0; i < rhv.digits.size() || carry; ++i) {
        lhv.digits[i] -= carry + (i < rhv.digits.size() ? rhv.digits[i] : 0);
        carry = lhv.digits[i] < 0;
        if (carry) lhv.digits[i] += 10;
    }
    lhv.StripZeros();
}

BigInteger& operator+=(BigInteger& lhv, const BigInteger& rhv) {
    if ((lhv.is_positive && rhv.is_positive) || (!lhv.is_positive && !rhv.is_positive)) {
        int carry = 0;
        lhv.digits.resize(std::max(lhv.digits.size(), rhv.digits.size()) + 1);
        for (size_t i = 0; i < lhv.digits.size(); ++i) {
            int sum = carry + (i < rhv.digits.size() ? rhv.digits[i] : 0);
            lhv.digits[i] += sum;
            carry = lhv.digits[i] / 10;
            lhv.digits[i] %= 10;
        }
    } else {
        auto l = lhv.Abs();
        auto r = rhv.Abs();
        if (l.Compare(r) >= 0) {
            lhv.Diff(lhv, rhv);
            if (l < r) {
                lhv.is_positive = (lhv.is_positive + 1) % 2;
            }
        } else {
            BigInteger tmplhv;
            BigInteger tmprhv;
            tmplhv = rhv;
            tmprhv = lhv;
            tmplhv.Diff(tmplhv, tmprhv);
            if (l < r) {
                tmplhv.is_positive = (lhv.is_positive + 1) % 2;
            }
            lhv = tmplhv;
        }
    }
    lhv.StripZeros();
    return lhv;
}

BigInteger& operator-=(BigInteger& lhv, const BigInteger& rhv) {    // Не дает 0 - rhv
    BigInteger temporary;
    temporary = rhv;
    temporary.is_positive = (rhv.is_positive + 1) % 2;
    lhv += temporary;
    return lhv;
}

BigInteger& operator%=(BigInteger& lhv, const BigInteger& rhv) {
    lhv = lhv % rhv;
    return lhv;
}

BigInteger& operator*=(BigInteger& lhv, const BigInteger& rhv) {
    lhv = lhv * rhv;
    return lhv;
}

BigInteger& operator/=(BigInteger& lhv, const BigInteger& rhv) {
    lhv = lhv / rhv;
    return lhv;
}

bool operator==(const BigInteger& lhv, const BigInteger& rhv) {
    return lhv.Compare(rhv) == 0;
}

bool operator!=(const BigInteger& lhv, const BigInteger& rhv) {
    return !(lhv == rhv);
}

bool operator<=(const BigInteger& lhv, const BigInteger& rhv) {
    return (lhv < rhv || lhv == rhv);
}

bool operator>=(const BigInteger& lhv, const BigInteger& rhv) {
    return (lhv > rhv || lhv == rhv);
}

bool operator<(const BigInteger& lhv, const BigInteger& rhv) {
    return lhv.Compare(rhv) == -1;
}

bool operator>(const BigInteger& lhv, const BigInteger& rhv) {
    return lhv.Compare(rhv) == 1;
}

std::ostream& operator<<(std::ostream& stream, const BigInteger& big_int) {
    stream << big_int.toString();
    return stream;
}

std::istream& operator>>(std::istream& stream, BigInteger& big_int) {
    std::string str;
    if (stream.peek() != EOF) {
        stream >> str;
    }
    big_int = BigInteger(str);
    //std::cerr << str;
    return stream;
}


int BigInteger::Compare(const BigInteger& rhv) const {
    int res_fix = 1;

    if (is_positive && !rhv.is_positive) return 1;
    if (!is_positive && rhv.is_positive) return -1;
    if (!is_positive && !rhv.is_positive) res_fix = -1;

    for (ssize_t i = std::max(digits.size(), rhv.digits.size()) - 1; i >= 0; --i) {
        if (ssize_t(digits.size()) < i + 1 && !(ssize_t(rhv.digits.size()) < i + 1)) {
            if (rhv.digits[i] != 0) {
                return -1 * res_fix;
            }
            continue;
        }
        if (ssize_t(rhv.digits.size()) < i + 1 && !(ssize_t(digits.size()) < i + 1)) {
            if (digits[i] != 0) {
                return 1 * res_fix;
            }
            continue;
        }
        if (digits[i] < rhv.digits[i]) return -1 * res_fix;
        if (digits[i] > rhv.digits[i]) return res_fix;
    }

    return 0;
}

void BigInteger::AddZeros(size_t count) {
    digits.resize(digits.size() + count);
}

BigInteger BigInteger::Abs() const {
    BigInteger result;
    result = *this;
    result.is_positive = true;
    return result;
}

const BigInteger BigInteger::operator-() {
    is_positive = (is_positive + 1) % 2;
    return *this;
}

#endif //BIGINT_BIGINTEGER_H
