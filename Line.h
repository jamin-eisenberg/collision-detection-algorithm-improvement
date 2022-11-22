#ifndef PROJECT_LINE_H
#define PROJECT_LINE_H

#include "Vector2D.h"
#include "Screen.h"

// represents a line with two endpoints
class Line {
public:
    Vector2D p1;
    Vector2D p2;

    // creates a line with the given vectors as endpoints
    Line(Vector2D *, Vector2D *);

    ~Line();

    // translates the vector by the given vector
    void add(Vector2D*);

    // translates the vector by the negative of the given vector
    void sub(Vector2D*);

    // scales the vector by a constant factor
    void mult(double);

    // rotates the vector by a given angle
    void rotate(double);

    // return whether this line is colliding with the given line
    bool isColliding(Line*);

    // draws this line to the given screen
    void draw(Screen*, bool);
};

#endif
