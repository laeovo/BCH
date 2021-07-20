#include <iostream>
#include <vector>

#include "GF2.hpp"
#include "util.hpp"
#include "Polynom.hpp"

using namespace std;

const size_t power(const size_t b, const size_t e) {
    if (e == 0) {
        return 1;
    }
    assert(e > 0);
    int output{1};
    for (size_t i = 0; i < e; ++i) {
        output *= b;
    }
    return output;
}

const vector<Polynom> polynomgenerator(const size_t minGrad, const size_t maxGrad) {
    const vector<Polynom> polynome{polynomgenerator(maxGrad)};
    vector<Polynom> neuePolynome{};
    for (const Polynom& p : polynome) {
        if (p.grad() >= minGrad) {
            neuePolynome.push_back(p);
        }
    }
    return neuePolynome;
}

const vector<Polynom> polynomgenerator(const size_t maxGrad) {
    if (maxGrad > 0) {
        vector<Polynom> neuePolynome{};
        for (const Polynom& p : polynomgenerator(maxGrad - 1)) {
            vector<GF2> koeffizienten0{p.getKoeff()};
            vector<GF2> koeffizienten1{p.getKoeff()};
            reverse(koeffizienten0.begin(), koeffizienten0.end());
            reverse(koeffizienten1.begin(), koeffizienten1.end());
            koeffizienten0.push_back(0);
            koeffizienten1.push_back(1);
            reverse(koeffizienten0.begin(), koeffizienten0.end());
            reverse(koeffizienten1.begin(), koeffizienten1.end());
            neuePolynome.push_back(koeffizienten0);
            neuePolynome.push_back(koeffizienten1);
        }
        return neuePolynome;
    }
    return {Polynom({0}), Polynom({1})};
}

const Polynom getIrreduziblesPolynomVomGrad(const size_t grad) {
    for (const Polynom& p : polynomgenerator(grad, grad)) {
        if (!p.checkReduzibel()) {
            return p;
        }
    }
    cout << "Kein irreduzibles Polynom vom Grad " << grad << " gefunden" << endl;
    assert(false);
    return Polynom();
}
