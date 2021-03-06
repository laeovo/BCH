#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "BCHCode.hpp"
#include "Polynom.hpp"
#include "util.hpp"
#include "Matrix.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    const Polynom modularPolynom{{1, 1, 0, 0, 1}};
    const GF2m koerper{modularPolynom};
    const BCHCode code{koerper, 2};
    const Vektor<GF2> codewort{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
//    cout << "Codewort: ";
//    codewort.print(true);
    for (size_t stelle1 = 0; stelle1 < 15; ++stelle1) {
        for (size_t stelle2 = 0; stelle2 < 15; ++stelle2) {
            vector<GF2> fehler(15);
            fehler[stelle1] = 1;
            fehler[stelle2] = 1;
            const Vektor<GF2> empfangen{codewort+Vektor<GF2>(fehler)};
            const Vektor<GF2> korrigiert{code.dekodieren(empfangen)};
            if (korrigiert != codewort) {
                cout << "Fehler!" << endl;
            }
        }
    }

//    const Polynom modularPolynom{{1, 1, 0, 0, 1}};
//    const GF2m koerper{modularPolynom};
//    const BCHCode code{koerper, 2};
//    const Matrix<GF2> H{code.getPruefmatrix()};
//
//    H.print();
//
//    const Vektor<GF2> a{{1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1}};
//    (H*a).print(true);
//    const Vektor<GF2> b{{0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1}};
//    (H*b).print(true);
//    const Vektor<GF2> c{{0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1}};
//    (H*c).print(true);
}
