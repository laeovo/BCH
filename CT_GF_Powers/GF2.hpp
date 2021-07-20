#ifndef GF2_hpp
#define GF2_hpp

#include <stdio.h>

class GF2 {
public:
    GF2();
    GF2(int value);
    GF2(bool value);
    const GF2 operator+(const GF2& other) const;
    const GF2 operator-(const GF2& other) const;
    const GF2 operator*(const GF2& other) const;
    const bool operator==(const int other) const;
    const bool operator!=(const int other) const;
    const bool operator==(const GF2 other) const;
    const bool operator!=(const GF2 other) const;
    const std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const GF2& gf2) {
        if (gf2.value) {
            os << "1";
        }
        else {
            os << "0";
        }
        return os;
    }
private:
    bool value;
};

#endif /* GF2_hpp */
