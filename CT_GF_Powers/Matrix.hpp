#ifndef Matrix_hpp
#define Matrix_hpp

#include <iostream>
#include <stdio.h>
#include <vector>

#include "Vektor.hpp"

template<class T>
class Matrix {
public:
    Matrix();
    Matrix(const std::vector<Vektor<T>>& input);
    const Vektor<T> operator*(const Vektor<T>& other) const;
    const Matrix<T> operator*(const Matrix<T>& other) const;
    const size_t anzahlZeilen() const;
    const size_t anzahlSpalten() const;
    const Vektor<T> getSpalte(const size_t n) const;
    const Matrix<T> transponiert() const;
    void print() const;
private:
    std::vector<Vektor<T>> zeilen;
};

template<class T>
Matrix<T>::Matrix() : zeilen{{0}} {}

template<class T>
Matrix<T>::Matrix(const std::vector<Vektor<T>>& input) : zeilen(input) {
    for (size_t i = 1; i < this->zeilen.size(); ++i) {
        assert(this->zeilen[i].size() == this->zeilen[0].size());
    }
}

template<class T>
const Vektor<T> Matrix<T>::operator*(const Vektor<T>& other) const {
    assert(this->anzahlSpalten() == other.size());
    std::vector<T> output(this->zeilen.size());
    for (size_t i = 0; i < this->zeilen.size(); ++i) {
        output[i] = this->zeilen[i].skalarprodukt(other);
    }
    return output;
}

template<class T>
const Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    assert(this->anzahlSpalten() == other.anzahlZeilen());
    std::vector<Vektor<T>> output(this->anzahlZeilen());
    for (size_t i = 0; i < this->anzahlZeilen(); ++i) {
        output[i] = other.transponiert() * this->zeilen[i];
    }
    return output;
}

template<class T>
const size_t Matrix<T>::anzahlZeilen() const {
    return this->zeilen.size();
}

template<class T>
const size_t Matrix<T>::anzahlSpalten() const {
    return this->zeilen[0].size();
}

template<class T>
const Vektor<T> Matrix<T>::getSpalte(const size_t n) const {
    assert(n < this->anzahlSpalten());
    std::vector<T> output(this->anzahlZeilen());
    for (size_t i = 0; i < this->anzahlZeilen(); ++i) {
        output[i] = this->zeilen[i][n];
    }
    return output;
}

template<class T>
const Matrix<T> Matrix<T>::transponiert() const {
    std::vector<Vektor<T>> output(this->anzahlSpalten());
    for (size_t i = 0; i < this->anzahlSpalten(); ++i) {
        output[i] = this->getSpalte(i);
    }
    return output;
}

template<class T>
void Matrix<T>::print() const {
    for (size_t i = 0; i < this->anzahlZeilen(); ++i) {
        for (size_t j = 0; j < this->zeilen[i].size(); ++j) {
            std::cout << this->zeilen[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

#endif /* Matrix_hpp */
