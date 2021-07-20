#include <string>

#include "GF2.hpp"

using namespace std;

GF2::GF2() : value(false) {}

GF2::GF2(int value) {
    if (value == 0) {
        this->value = false;
    }
    else {
        this->value = true;
    }
}

GF2::GF2(bool value) : value(value) {}

const GF2 GF2::operator+(const GF2& other) const {
    return this->value ^ other.value;
}

const GF2 GF2::operator-(const GF2& other) const {
    return *this+other;
}

const GF2 GF2::operator*(const GF2& other) const {
    return this->value && other.value;
}

const bool GF2::operator==(const int other) const {
    if (other == 1 && this->value) {
        return true;
    }
    else if (other == 0 && !this->value) {
        return true;
    }
    return false;
}

const bool GF2::operator!=(const int other) const {
    return !(*this == other);
}

const bool GF2::operator==(const GF2 other) const {
    return this->value == other.value;
}

const bool GF2::operator!=(const GF2 other) const {
    return !(*this == other);
}

const string GF2::toString() const {
    return this->value ? "1" : "0";
}
