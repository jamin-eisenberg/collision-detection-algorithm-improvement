#ifndef PROJECT_VECTOR2D_H
#define PROJECT_VECTOR2D_H

#include "Screen.h"

// represents a 2D vector for use in physics
class Vector2D {
public:
    double x;
    double y;

    Vector2D();

    Vector2D(double, double);

    ~Vector2D();

    // translates the vector by the given vector
    void add(Vector2D*);

    // translates the vector by the negative of the given vector
    void sub(Vector2D*);

    // scales the vector by a constant factor
    void mult(double);

    // rotates the vector by a given angle
    void rotate(double);

    // draws this line to the given screen
    void draw(Screen*, bool);

    // copies this vector to a new vector
    Vector2D copy();
};

#endif
