#ifndef Polynom_hpp
#define Polynom_hpp

#include <stdio.h>
#include <vector>

#include "GF2.hpp"

class Polynom {
public:
    Polynom();
    Polynom(std::vector<GF2> koeffizienten);
    const Polynom operator+(const Polynom& other) const;
    const Polynom operator-(const Polynom& other) const;
    const Polynom operator*(const Polynom& other) const;
    const Polynom mod(const Polynom& other) const;
    const Polynom pow(const size_t i) const;
    const bool operator==(const Polynom& other) const;
    const bool operator==(const std::vector<GF2>& other) const;
    const bool operator==(const size_t other) const;
    const GF2 operator[](const size_t i) const;
    const size_t grad() const;
    void print() const;
    void printLong(const bool alleMonome = false) const;
    const std::vector<GF2> getKoeff(const bool reihenfolgeUmkehren = false) const;
    const bool checkReduzibel() const;
    const Polynom operator()(const Polynom& p) const;
private:
    std::vector<GF2> koeffizienten;
    void kuerzen();
};

const Polynom polynomVomGrad(const size_t grad);

#endif /* Polynom_hpp */
