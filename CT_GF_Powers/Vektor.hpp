#ifndef Vektor_hpp
#define Vektor_hpp

#include <iostream>
#include <stdio.h>
#include <vector>

template<class T>
class Vektor {
public:
    Vektor();
    Vektor(const std::vector<T> input);
    const Vektor<T> operator+(const Vektor<T>& other) const;
    const Vektor<T>& operator*=(const T& lambda);
    const T& operator[](const size_t n) const;
    const T skalarprodukt(const Vektor<T>& other) const;
    const size_t size() const;
    void print(const bool alsZeilenvektor = false) const;
private:
    std::vector<T> data;
};

template<class T>
Vektor<T>::Vektor() : data({}) {}

template<class T>
Vektor<T>::Vektor(const std::vector<T> input) : data(input) {}

template<class T>
const Vektor<T> Vektor<T>::operator+(const Vektor<T>& other) const {
    assert(this->size() == other.size());
    std::vector<T> output(this->size());
    for (size_t i = 0; i < this->size(); ++i) {
        output[i] = this->data[i] + other.data[i];
    }
    return output;
}

template<class T>
const Vektor<T>& Vektor<T>::operator*=(const T& lambda) {
    for (T& eintrag : this->data) {
        eintrag = lambda * eintrag;
    }
    return *this;
}

template<class T>
const size_t Vektor<T>::size() const {
    return this->data.size();
}

template<class T>
const T& Vektor<T>::operator[](const size_t n) const {
    assert(n < this->size());
    return this->data[n];
}

template<class T>
const T Vektor<T>::skalarprodukt(const Vektor<T>& other) const {
    assert(this->size() == other.size());
    T output;
    for (size_t i = 0; i < this->size(); ++i) {
        output = output + (this->data[i]*other.data[i]);
    }
    return output;
}

template<class T>
void Vektor<T>::print(const bool alsZeilenvektor) const {
    if (alsZeilenvektor) {
        std::cout << "( ";
    }
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->data[i];
        if (alsZeilenvektor) {
            std::cout << " ";
        }
        else {
            std::cout << std::endl;
        }
    }
    if (alsZeilenvektor) {
        std::cout << ")" << std::endl;
    }
}

#endif /* Vektor_hpp */
