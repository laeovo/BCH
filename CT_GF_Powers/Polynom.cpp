#include <algorithm>
#include <iostream>

#include "Polynom.hpp"
#include "util.hpp"

using namespace std;

Polynom::Polynom() : koeffizienten{{0}} {}

Polynom::Polynom(vector<GF2> koeffizienten) : koeffizienten(koeffizienten) {
    if (this->koeffizienten.size() == 0) {
        this->koeffizienten = {0};
    }
    this->kuerzen();
}

const Polynom Polynom::operator+(const Polynom& other) const {
    vector<GF2> neueKoeffizienten(max(this->grad(), other.grad())+1);
    for (size_t i = 0; i < neueKoeffizienten.size(); ++i) {
        if (i < this->grad()+1) {
            neueKoeffizienten[i] = neueKoeffizienten[i] + this->koeffizienten[i];
        }
        if (i < other.grad()+1) {
            neueKoeffizienten[i] = neueKoeffizienten[i] + other.koeffizienten[i];
        }
    }
    return neueKoeffizienten;
}

const Polynom Polynom::operator-(const Polynom& other) const {
    return *this + other;
}

const Polynom Polynom::operator*(const Polynom& other) const {
    vector<GF2> neueKoeffizienten(this->grad() + other.grad() + 1);
    for (size_t i = 0; i <= this->grad(); ++i) {
        for (size_t j = 0; j <= other.grad(); ++j) {
            neueKoeffizienten[i+j] = neueKoeffizienten[i+j] + (this->koeffizienten[i] * other.koeffizienten[j]);
        }
    }
    return neueKoeffizienten;
}

const Polynom Polynom::mod(const Polynom& other) const {
    assert(!(other == 0));
    if (other == 1) {
        return Polynom();
    }
    Polynom rest = *this;
    while (other.grad() <= rest.grad()) {
        rest = rest - (polynomVomGrad(rest.grad() - other.grad()) * other);
    }
    return rest;
}

const Polynom Polynom::pow(const size_t i) const {
    Polynom power{{1}};
    for (size_t j = 0; j < i; ++j) {
        power = power * *this;
    }
    return power;
}

const bool Polynom::operator==(const Polynom& other) const {
    if (this->koeffizienten.size() != other.koeffizienten.size()) {
        return false;
    }
    for (size_t i = 0; i < this->koeffizienten.size(); ++i) {
        if (this->koeffizienten[i] != other.koeffizienten[i]) {
            return false;
        }
    }
    return true;
}

const bool Polynom::operator==(const std::vector<GF2>& other) const {
    return *this == Polynom(other);
}

const bool Polynom::operator==(const size_t other) const {
    if (other == 0) {
        return *this == Polynom({0});
    }
    else if (other == 1) {
        return *this == Polynom({1});
    }
    else {
        cout << "Sinnloser Vergleich: Polynom == " << other << endl;
    }
    return false;
}

const GF2 Polynom::operator[](const size_t i) const {
    if (i >= this->koeffizienten.size()) {
        return 0;
    }
    return this->koeffizienten[i];
}

const size_t Polynom::grad() const {
    for (size_t i = this->koeffizienten.size()-1; i < this->koeffizienten.size(); --i) {
        if (this->koeffizienten[i] == 1) {
            return i;
        }
    }
    return 0;
}

void Polynom::print() const {
    for (size_t i = this->koeffizienten.size()-1; i < this->koeffizienten.size(); --i) {
        if (this->koeffizienten[i] == 1) {
            cout << "1";
        }
        else {
            cout << "0";
        }
        cout << " ";
    }
    cout << endl;
}

void Polynom::printLong(const bool alleMonome) const {
    if (*this == 0) {
        cout << "Null-Polynom" << endl;
        return;
    }
    for (size_t i = this->koeffizienten.size()-1; i < this->koeffizienten.size(); --i) {
        if (this->koeffizienten[i] == 1 || alleMonome) {
            cout << "x^" << i;
            size_t verbleibendeMonome{0};
            for (size_t j = 0; j < i; ++j) {
                if (this->koeffizienten[j] == 1) {
                    ++verbleibendeMonome;
                }
            }
            if (verbleibendeMonome > 0) {
                cout << " + ";
            }
        }
    }
    cout << endl;
}

const std::vector<GF2> Polynom::getKoeff(const bool reihenfolgeUmkehren) const {
    if (!reihenfolgeUmkehren) {
        return this->koeffizienten;
    }
    vector<GF2> koeffReverse{this->koeffizienten};
    reverse(koeffReverse.begin(), koeffReverse.end());
    return koeffReverse;
}

const bool Polynom::checkReduzibel() const {
    if (this->grad() <= 1) {
        return false;
    }
    // TODO: nur bis sqrt(this->grad) testen
    for (const Polynom& p : polynomgenerator(this->grad()-1)) {
        if (p.grad() > 0) {
            if (p.grad() > 0 && this->mod(p) == 0) {
                return true;
            }
        }
    }
    return false;
}

void Polynom::kuerzen() {
    while(this->koeffizienten.back() == 0 && this->koeffizienten.size() > 1) {
        this->koeffizienten.pop_back();
    }
}

const Polynom polynomVomGrad(const size_t grad) {
    vector<GF2> koeffizienten = vector<GF2>(grad);
    koeffizienten.push_back(1);
    return koeffizienten;
}

const Polynom Polynom::operator()(const Polynom& p) const {
//    cout << "Auswerten" << endl;
//    cout << "This : ";
//    this->print();
//    cout << "Other: ";
//    p.print();
    Polynom output;
//    cout << "Gehe durch Koeffizienten" << endl;
    for (size_t i = 0; i < this->koeffizienten.size(); ++i) {
//        cout << "i = " << i << ", Koeff = " << this->koeffizienten[i].toString() << endl;
        if (this->koeffizienten[i] == 1) {
//            cout << " Power : ";
//            p.pow(i).print();
            output = output + p.pow(i);
//            cout << " Output: ";
//            output.print();
        }
    }
    return output;
}
