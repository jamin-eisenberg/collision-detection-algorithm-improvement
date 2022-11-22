#include <algorithm>
#include <iostream>
#include "Line.h"

using namespace std;

// creates a line with the given vectors as endpoints
Line::Line(Vector2D *p1, Vector2D *p2) {
    this->p1 = *p1;
    this->p2 = *p2;
}

Line::~Line() = default;

// translates the vector by the given vector
void Line::add(Vector2D *other) {
    p1.add(other);
    p2.add(other);
}

// translates the vector by the negative of the given vector
void Line::sub(Vector2D *other) {
    p1.sub(other);
    p2.sub(other);
}

// scales the vector by a constant factor
void Line::mult(double k){
    p1.mult(k);
    p2.mult(k);
}

// rotates the vector by a given angle
void Line::rotate(double angle) {
    p1.rotate(angle);
    p2.rotate(angle);
}

// return whether this line is colliding with the given line
bool Line::isColliding(Line *other) {
    // Using an algorithm like the one found here: https://gamedev.stackexchange.com/questions/26004/how-to-detect-2d-line-on-line-collision

    double denominator = ((p2.x - p1.x) * (other->p2.y - other->p1.y)) - ((p2.y - p1.y) * (other->p2.x - other->p1.x));
    double numerator1 =
            ((p1.y - other->p1.y) * (other->p2.x - other->p1.x)) - ((p1.x - other->p1.x) * (other->p2.y - other->p1.y));
    double numerator2 = ((p1.y - other->p1.y) * (p2.x - p1.x)) - ((p1.x - other->p1.x) * (p2.y - p1.y));

    if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

    double r = numerator1 / denominator;
    double s = numerator2 / denominator;

    return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

// draws this line to the given screen
void Line::draw(Screen *s, bool isWhite) {
    // Using the Bresenham line-drawing algorithm found here: https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html

    int x0 = p1.x, x1 = p2.x;
    int y0 = p1.y, y1 = p2.y;
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (true) {
        s->writePixel(x0, y0, isWhite);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}