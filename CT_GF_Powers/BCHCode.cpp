#include <iostream>

#include "BCHCode.hpp"
#include "util.hpp"

using namespace std;

BCHCode::BCHCode(const GF2m& koerper, const size_t zuKorrigierendeBits) : koerper(koerper) {
    this->setzePrimitiveElemente(zuKorrigierendeBits);
    this->setzePruefmatrix();
    this->setzeGeneratorPolynom();
    this->n = power(2, this->koerper.getM())-1;
    this->k = this->n - this->koerper.getM() * zuKorrigierendeBits;
    this->zuKorrigierendeBits = zuKorrigierendeBits;
}

void BCHCode::setzePrimitiveElemente(const size_t zuKorrigierendeBits) {
    vector<size_t> startPotenzen{};
    vector<size_t> benutzt{};
    for (size_t i = 1; i < power(2, this->koerper.getM())-1; ++i) {
        if (find(benutzt.begin(), benutzt.end(), i) == benutzt.end()) {
            startPotenzen.push_back(i);
            if (startPotenzen.size() >= zuKorrigierendeBits) {
                break;
            }
            for (size_t j = 0; j < this->koerper.getM(); ++j) {
                benutzt.push_back((i*power(2, j))%(power(2, this->koerper.getM())-1));
            }
        }
    }
    for (size_t i = 0; i < startPotenzen.size(); ++i) {
        this->alphas.push_back(this->koerper.getPolynom(startPotenzen[i]));
    }
}

void BCHCode::setzePruefmatrix() {
    vector<Vektor<GF2>> spaltenDerHMatrix{};
    vector<Vektor<size_t>> spaltenDerHexpMatrix{};
    for (size_t spaltenIndex = 0; spaltenIndex < power(2, this->koerper.getM())-1; ++spaltenIndex) {
        vector<GF2> spalteDerHMatrix(this->koerper.getM() * this->alphas.size());
        vector<size_t> spalteDerHexpMatrix(this->alphas.size());
        for (size_t zeilenIndex = 0; zeilenIndex < this->alphas.size(); ++zeilenIndex) {
            const Polynom potenz{this->koerper.mod(this->alphas[zeilenIndex].pow(spaltenIndex%(power(2, this->koerper.getM())-1)))};
            const size_t exponent{this->koerper.getExponent(potenz)};
            for (size_t j = 0; j < this->koerper.getM(); ++j) {
                spalteDerHMatrix[zeilenIndex*this->koerper.getM() + j] = potenz[this->koerper.getM()-j-1];
            }
            spalteDerHexpMatrix[zeilenIndex] = exponent;
        }
        spaltenDerHMatrix.push_back(Vektor<GF2>(spalteDerHMatrix));
        spaltenDerHexpMatrix.push_back(spalteDerHexpMatrix);
    }
    reverse(spaltenDerHMatrix.begin(), spaltenDerHMatrix.end());
    reverse(spaltenDerHexpMatrix.begin(), spaltenDerHexpMatrix.end());
    Matrix<GF2> HTransponiert{spaltenDerHMatrix};
    Matrix<size_t> HexpTransponiert{spaltenDerHexpMatrix};
    this->H = HTransponiert.transponiert();
    this->Hexp = HexpTransponiert.transponiert();
}

void BCHCode::setzeGeneratorPolynom() {
    vector<Polynom> minimalPolynome{};
    for (const Polynom& alpha : this->alphas) {
        minimalPolynome.push_back(this->koerper.getMinimalpolynomBruteForce(alpha));
    }
    assert(minimalPolynome.size() > 0);
    Polynom generatorPolynom{minimalPolynome[0]};
    for (size_t i = 1; i < minimalPolynome.size(); ++i) {
        if (generatorPolynom.mod(minimalPolynome[i]) != 0) {
            generatorPolynom = generatorPolynom * minimalPolynome[i];
        }
    }
    this->g = generatorPolynom;
}

const Matrix<GF2>& BCHCode::getPruefmatrix() const {
    return this->H;
}

const Matrix<size_t> BCHCode::getPruefmatrixExp() const {
    return this->Hexp;
}

const Polynom& BCHCode::getGeneratorPolynom() const {
    return this->g;
}

const Vektor<GF2> BCHCode::computeSyndrom(const Vektor<GF2>& empfangen) const {
    assert(empfangen.size() == this->n);
    return this->H*empfangen;
}

const Vektor<size_t> BCHCode::computeSyndromExp(const Vektor<GF2>& empfangen) const {
    const Vektor<GF2> syndromBinaer{this->computeSyndrom(empfangen)};
//    cout << "Syndrom binär:";
//    syndromBinaer.print(true);
    vector<size_t> exponenten{};
    for (size_t i = 0; i < this->zuKorrigierendeBits; ++i) {
        vector<GF2> koeffizienten{};
        for (size_t j = this->koerper.getM()-1; j < this->koerper.getM(); --j) {
            koeffizienten.push_back(syndromBinaer[i*this->koerper.getM() + j]);
        }
        const size_t exponent{this->koerper.getExponent(Polynom(koeffizienten))};
        exponenten.push_back(exponent);
    }
    return exponenten;
}

const Vektor<GF2> BCHCode::getCodewort(const Polynom& datenwort) const {
    assert(datenwort.grad() == this->k-1);
    const Polynom codewortAlsPolynom{datenwort * this->g};
    return codewortAlsPolynom;
}

//const Wort BCHCode::getCodewort(const Wort& datenwort) const {
//    assert(datenwort.getLaenge() == this->k);
//
//}

const vector<Vektor<GF2>> BCHCode::getAlleCodewoerter() const {
    vector<Vektor<GF2>> codewoerter{};
    for (const Polynom& datenwort : polynomgenerator(this->k-1, this->k-1)) {
        codewoerter.push_back(getCodewort(datenwort));
    }
    return codewoerter;
}

const vector<size_t> BCHCode::computeFehlerstellen(const Vektor<GF2>& empfangen) const {
//    cout << "Empfangen: ";
//    empfangen.print(true);
    const Vektor<size_t> S{this->computeSyndromExp(empfangen)};
//    cout << "S1 = a^" << S[0] << ", S3 = a^" << S[1] << endl;
    switch (this->zuKorrigierendeBits) {
        case 1:
            {
                const Polynom S1{this->koerper.getPolynom(S[0])};
                const Polynom sigma11{S1};
                return {this->n - this->koerper.getExponent(sigma11) - 1};
            }
        case 2:
            {
                const Polynom S1{this->koerper.getPolynom(S[0])};
                const Polynom S3{this->koerper.getPolynom(S[1])};
                const Polynom sigma21{S1};
                const Polynom sigma22{this->koerper.geteilt(S1*S1*S1+S3, S1)};
//                cout << "sigma21: a^" << this->koerper.getExponent(sigma21) << ", sigma22: a^" << this->koerper.getExponent(sigma22) << endl;
                if (sigma21 == 0) {
                    return {this->koerper.getExponent(this->koerper.sqrt(sigma22))};
                }
                else {
                    const Polynom C_i{this->koerper.geteilt(sigma22, sigma21*sigma21)};
//                    cout << "C_i: a^" << this->koerper.getExponent(C_i) << endl;
                    vector<size_t> loesungen{};
                    for (const Polynom& y : polynomgenerator(this->koerper.getM()-1)) {
                        if (this->koerper.mod((y*y) + y + C_i) == 0) {
                            const Polynom zuKorrigierendesBit{sigma21 * y};
                            if (zuKorrigierendesBit != 0) {
                                loesungen.push_back(this->n - this->koerper.getExponent(sigma21 * y) - 1);
                            }
                        }
                    }
                    return loesungen;
                }
            }
            break;
            
        default:
            return {};
            break;
    }
    return {};
}
