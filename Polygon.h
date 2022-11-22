#ifndef PROJECT_POLYGON_H
#define PROJECT_POLYGON_H

#include "Vector2D.h"
#include "Line.h"
#include "Screen.h"
#include <vector>

using namespace std;

// represents a polygon, in the traditional, geometric sense
class Polygon {
private:
    Vector2D pos;
    Vector2D vel;
    double angle;
    double angleVel;
    double inv_mass;
    vector<Line> *lines;

    friend class CollisionPair;

    // initializes a polygon with all of the necessary information, called by constructors
    void initPolygon(Vector2D *, Vector2D *, double, double, double, vector<Line> *, int);

public:
    Vector2D aabbMin;
    Vector2D aabbMax;

    int id;

    Polygon();

    Polygon(Vector2D *, Vector2D *, double, double, double, vector<Line> *, int);

    Polygon(Vector2D *, Vector2D *, double, double, double, int, int);

    ~Polygon();

    // returns whether this Polygon is colliding with the given one
    bool isColliding(Polygon *);

    // updates the AABB, position, angle, and lines of this Polygon, once per tick
    void update(Screen *);

    // draws this Polygon to the given screen
    void draw(Screen *, bool);

    // compares two Polygons by their ids
    bool operator<(const Polygon &p2) const;
};

#endif
