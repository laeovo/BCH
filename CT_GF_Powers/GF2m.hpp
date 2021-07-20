#ifndef GF2m_hpp
#define GF2m_hpp

#include <stdio.h>

#include "Polynom.hpp"
#include "Matrix.hpp"

class GF2m {
public:
    GF2m(const Polynom& modularPolynom);
    const Polynom plus(const Polynom& a, const Polynom& b) const;
    const Polynom minus(const Polynom& a, const Polynom& b) const;
    const Polynom mal(const Polynom& a, const Polynom& b) const;
    const Polynom geteilt(const Polynom& a, const Polynom& b) const;
    const Polynom sqrt(const Polynom& p) const;
    const size_t ordnung(const Polynom& p) const;
    const bool istPrimitiv(const Polynom& p) const;
    
    const size_t getExponent(const Polynom& p) const;
    const Polynom getPolynom(const size_t exponent) const;
    const Polynom getInvers(const Polynom& p) const;
    const Polynom mod(const Polynom& p) const; // TODO: private machen
    const Polynom getMinimalpolynomBruteForce(const Polynom& p) const;
    
    const size_t getM() const;
    const Polynom& getAlpha() const;
private:
    const Polynom modularPolynom;
    const size_t m;
    Polynom alpha;
    void setPrimitivesElement();
};

#endif /* GF2m_hpp */
