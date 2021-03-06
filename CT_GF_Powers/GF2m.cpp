#include <iostream>
#include <vector>

#include "GF2m.hpp"
#include "util.hpp"

using namespace std;

GF2m::GF2m(const Polynom& modularPolynom) : modularPolynom(modularPolynom), m(modularPolynom.grad()) {
    assert(!this->modularPolynom.checkReduzibel());
    this->setPrimitivesElement();
    this->setzePotenztabellen();
}

const Polynom GF2m::plus(const Polynom& a, const Polynom& b) const {
    return this->mod(a+b);
}

const Polynom GF2m::minus(const Polynom& a, const Polynom& b) const {
    return this->mod(a-b);
}

const Polynom GF2m::mal(const Polynom& a, const Polynom& b) const {
    return this->mod(a*b);
}

const Polynom GF2m::geteilt(const Polynom& a, const Polynom& b) const {
    return this->mod(a * this->getInvers(b));
}

const Polynom GF2m::sqrt(const Polynom& p) const {
    assert(p.grad() < this->m);
    for (const Polynom& moeglicheWurzel : polynomgenerator(this->m-1)) {
        if (this->mal(moeglicheWurzel, moeglicheWurzel) == p) {
            return moeglicheWurzel;
        }
    }
    return Polynom();
}

const size_t GF2m::ordnung(const Polynom& p) const {
    assert(p != 0);
    if (p.grad() >= m) {
        return this->ordnung(this->mod(p));
    }
    Polynom potenz{{1}};
    for (size_t i = 1; i <= power(2, this->m)-1; ++i) {
        potenz = this->mod(potenz * p);
        if (potenz == 1) {
            return i;
        }
    }
    // Hier sollte man nicht landen
    assert(false);
    return 0;
}

const bool GF2m::istPrimitiv(const Polynom& p) const {
    return this->ordnung(p) == power(2, m) - 1;
}

const size_t GF2m::getExponent(const Polynom& p) const {
    if (p.grad() >= this->m) {
        return this->getExponent(this->mod(p));
    }
    for (size_t i = 0; i < this->polynome.size(); ++i) {
        if (this->polynome[i] == p) {
            return i;
        }
    }
    cout << "Polynom wurde nicht gefunden! p = ";
    p.printLong();
    assert(false);
    return 0;
}

const Polynom GF2m::getPolynom(const size_t exponent) const {
    assert(exponent < power(2, this->m));
    return this->polynome[exponent];
}

const Polynom GF2m::getInvers(const Polynom& p) const {
    assert(p != 0);
    if (p == 1) {
        return Polynom({1});
    }
    const size_t exponent{this->getExponent(p)};
    return this->getPolynom(power(2, this->m)-1 - exponent);
}

const Polynom GF2m::mod(const Polynom& p) const {
    return p.mod(this->modularPolynom);
}

void GF2m::setPrimitivesElement() {
    if (this->alpha != 0) {
        cout << "Es existiert bereits ein primitives Element: ";
        this->alpha.print();
        return;
    }
    const vector<Polynom> polynomPool{polynomgenerator(1, this->m-1)};
    size_t i{0};
    do {
        this->alpha = polynomPool[i++];
    }
    while (!this->istPrimitiv(this->alpha));
}



const Polynom GF2m::getMinimalpolynomBruteForce(const Polynom& p) const {
    const vector<Polynom> kandidaten{polynomgenerator(1, this->m)};
    size_t maxGrad{this->m};
    Polynom minimalPolynom;
    for (const Polynom& kandidat : kandidaten) {
        
        if (this->mod(kandidat(p)) == 0 && kandidat.grad() <= maxGrad) {
            minimalPolynom = kandidat;
            maxGrad = minimalPolynom.grad();
        }
    }
    if (minimalPolynom == 0) {
        cout << "Warnung: Minimalpolynom ist 0." << endl;
    }
    return minimalPolynom;
}

const size_t GF2m::getM() const {
    return this->m;
}

const Polynom& GF2m::getAlpha() const {
    return this->alpha;
}

void GF2m::setzePotenztabellen() {
    this->polynome = vector<Polynom>(power(2, this->m));
    for (size_t i = 0; i < power(2, this->m)-1; ++i) {
        this->polynome[i] = this->mod(this->alpha.pow(i));
    }
    this->polynome[power(2, this->m)-1] = Polynom({0});
}
