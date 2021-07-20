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
        if (!(generatorPolynom.mod(minimalPolynome[i]) == 0)) {
            generatorPolynom = generatorPolynom * minimalPolynome[i];
        }
    }
    this->g = generatorPolynom;
}

const Matrix<GF2>& BCHCode::getPruefmatrix() const {
    return this->H;
}

const Polynom& BCHCode::getGeneratorPolynom() const {
    return this->g;
}

const Vektor<GF2> BCHCode::computeSyndrom(const Polynom& empfangen) const {
    vector<GF2> koeffizienten{empfangen.getKoeff()};
    while (koeffizienten.size() < this->n) {
        koeffizienten.push_back(0);
    }
    reverse(koeffizienten.begin(), koeffizienten.end());
    const Vektor<GF2> alsVektor{koeffizienten};
    return this->H*alsVektor;
}

const Vektor<size_t> BCHCode::computeSyndromExp(const Polynom& empfangen) const {
    // TODO: funktioniert noch nicht
    const Vektor<GF2> syndrom{this->computeSyndrom(empfangen)};
    // TODO: syndrom.size() asserten
    vector<size_t> exponenten{};
    for (size_t i = 0; i < this->zuKorrigierendeBits; ++i) {
        vector<GF2> koeffizienten{};
        for (size_t j = this->koerper.getM()-1; j < this->koerper.getM(); --j) {
            koeffizienten.push_back(syndrom[this->koerper.getM()*i+j]);
        }
        const Polynom polynom{koeffizienten};
        const size_t exponent{this->koerper.getExponent(polynom)};
        exponenten.push_back(exponent);
    }
    return exponenten;
}

const Polynom BCHCode::getCodewort(const Polynom& datenwort) const {
    assert(datenwort.grad() == this->k-1);
    return datenwort * this->g;
}

const vector<Polynom> BCHCode::getAlleCodewoerter() const {
    vector<Polynom> codewoerter{};
    for (const Polynom& datenwort : polynomgenerator(this->k-1, this->k-1)) {
        codewoerter.push_back(getCodewort(datenwort));
    }
    return codewoerter;
}

const vector<size_t> BCHCode::computeFehlerstellen(const Polynom& empfangen) const {
    const Vektor<size_t> S{this->computeSyndromExp(empfangen)};
    const Polynom S1{this->koerper.getPolynom(S[0])};
    const Polynom S3{this->koerper.getPolynom(S[1])};
    switch (this->zuKorrigierendeBits) {
        case 2:
            {
                const Polynom sigma21{S1};
                const Polynom sigma22{this->koerper.geteilt(this->koerper.mod(S1*S1*S1+S3), S1)};
                if (sigma21 == 0) {
                    return {this->koerper.getExponent(this->koerper.sqrt(sigma22))};
                }
                else {
                    const Polynom C_i{this->koerper.geteilt(sigma22, this->koerper.mod(sigma21*sigma21))};
                    vector<size_t> loesungen{};
                    for (const Polynom& y : polynomgenerator(this->koerper.getM()-1)) {
                        if (this->koerper.mod(y*y + y + C_i) == 0) {
                            loesungen.push_back(this->koerper.getExponent(y));
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
