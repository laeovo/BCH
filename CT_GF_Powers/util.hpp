#ifndef util_hpp
#define util_hpp

#include <stdio.h>

class Polynom;

const size_t power(const size_t b, const size_t e);

const std::vector<Polynom> polynomgenerator(const size_t maxGrad);
const std::vector<Polynom> polynomgenerator(const size_t minGrad, const size_t maxGrad);

const Polynom getIrreduziblesPolynomVomGrad(const size_t grad);

#endif /* util_hpp */
