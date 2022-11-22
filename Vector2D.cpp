#include <cmath>
#include <iostream>
#include <complex>
#include "Vector2D.h"

using namespace std;

Vector2D::Vector2D() {
    x = 0;
    y = 0;
}

Vector2D::Vector2D(double x, double y) {
    this->x = x;
    this->y = y;
}

Vector2D::~Vector2D() = default;

// translates the vector by the given vector
void Vector2D::add(Vector2D *other) {
    x += other->x;
    y += other->y;
}

// translates the vector by the negative of the given vector
void Vector2D::sub(Vector2D *other) {
    x -= other->x;
    y -= other->y;
}

// scales the vector by a constant factor
void Vector2D::mult(double k) {
    x *= k;
    y *= k;
}

typedef complex<double> point;

// rotates the vector by a given angle
void Vector2D::rotate(double ang) {

    // uses the imaginary plane for speed and convenience
    // also trigonometry was shrinking the vectors for some odd reason
    point p(x, y);

    point pNew = (p) * polar(1.0, ang);

    x = pNew.real();
    y = pNew.imag();
}

// draws this line to the given screen
void Vector2D::draw(Screen *s, bool isWhite) {
    s->writePixel(x, y, isWhite);
}

// copies this vector to a new vector
Vector2D Vector2D::copy() {
    return Vector2D(x, y);
}