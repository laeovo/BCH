#ifndef BCHCode_hpp
#define BCHCode_hpp

#include <stdio.h>

#include "GF2m.hpp"
#include "Matrix.hpp"
#include "Polynom.hpp"

class BCHCode {
public:
//    BCHCode(const size_t n, const size_t k); // TODO: Wär doch nice
    BCHCode(const GF2m& koerper, const size_t zuKorrigierendeBits);
    const Polynom get(const Polynom& empfangen) const;
    const Matrix<GF2>& getPruefmatrix() const;
    const Polynom& getGeneratorPolynom() const;
    const Vektor<GF2> computeSyndrom(const Polynom& empfangen) const;
    const Vektor<size_t> computeSyndromExp(const Polynom& empfangen) const;
    const Polynom getCodewort(const Polynom& datenwort) const;
    const std::vector<Polynom> getAlleCodewoerter() const;
    const std::vector<size_t> computeFehlerstellen(const Polynom& empfangen) const;
private:
    const GF2m& koerper;
    Matrix<GF2> H;
    Matrix<size_t> Hexp;
    Polynom g;
    std::vector<Polynom> alphas;
    size_t n;
    size_t k;
    size_t zuKorrigierendeBits;
    
    void setzePrimitiveElemente(const size_t zuKorrierendeBits);
    void setzePruefmatrix();
    void setzeGeneratorPolynom();
};

#endif /* BCHCode_hpp */
