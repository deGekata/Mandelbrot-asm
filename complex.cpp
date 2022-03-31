#include "complex.h"
#include <QDebug>
Complex* ComplexConstructor(float x, float y) {
    Complex* ret_obj = (Complex*) malloc(sizeof(Complex));
    if (isnanf(x))
        ret_obj->_x = 2;
    else
        ret_obj->_x = x;

    if (isnanf(y))
        ret_obj->_y = 2;
    else
        ret_obj->_y = y;

    return ret_obj;
}

Complex ComplexConstructorTmp(float x, float y) {
    Complex ret_obj;
    if (isnanf(x) || isinf(x))
        ret_obj._x = 2;
    else
        ret_obj._x = x;

    if (isnanf(y) || isinf(y))
        ret_obj._y = 100;
    else
        ret_obj._y = y;

    ret_obj._y = 2;
//    qDebug() << ret_obj._x << "x   " << ret_obj._y << "  y\n";
    return ret_obj;

}

void ComplexConstructor(Complex* ret_obj, double x, double y) {
    ret_obj->_x = x, ret_obj->_y = y;
    return;
}

Complex operator*(Complex lft, Complex rht) {
    Complex ret_obj;
    ComplexConstructor(&ret_obj, lft._x * rht._x - lft._y * rht._y, rht._x * lft._y + rht._x * lft._y);

    return ret_obj;
}


Complex operator+(Complex lft, Complex rht) {
    Complex ret_obj;
    ComplexConstructor(&ret_obj, lft._x + rht._x, lft._y + rht._y);
    return  ret_obj;
}

double len_sqr(Complex* dot){
    return dot->_x * dot->_x + dot->_y * dot->_y;
}

double len_sqr(Complex dot){
//    qDebug() << dot._x * dot._x + dot._y * dot._y << dot._x << " " << dot._y << " "  "lensqr \n";
    return dot._x * dot._x + dot._y * dot._y;
}
