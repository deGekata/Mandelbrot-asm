#ifndef COMPLEX_H
#define COMPLEX_H
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
struct Complex {

    double _x = 0, _y = 0;


};


Complex* ComplexConstructor(double x, double y);

Complex ComplexConstructorTmp(float x, float y);

void ComplexConstructor(Complex* ret_obj, double x, double y);

Complex operator*(Complex lft, Complex rht);

Complex operator+(Complex lft, Complex rht);

double len_sqr(Complex* dot);

double len_sqr(Complex dot);

#endif // COMPLEX_H
