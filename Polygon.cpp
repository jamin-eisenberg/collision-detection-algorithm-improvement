#include <stdlib.h>
#include <iostream>
#include "Polygon.h"

// initializes a polygon with all of the necessary information, called by constructors
void Polygon::initPolygon(Vector2D *pos, Vector2D *vel, double angle, double angleVel, double inv_mass,
                          vector<Line> *lines, int id) {
    this->pos = *pos;
    this->vel = *vel;
    this->angle = angle;
    this->angleVel = angleVel;
    this->inv_mass = inv_mass;

    this->aabbMin = *(new Vector2D());
    this->aabbMax = *(new Vector2D());

    this->lines = lines;

    this->id = id;
}

Polygon::Polygon() = default;

Polygon::Polygon(Vector2D *pos, Vector2D *vel, double angle, double angleVel, double inv_mass, vector<Line> *lines,
                 int id) {
    initPolygon(pos, vel, angle, angleVel, inv_mass, lines, id);
}

Polygon::Polygon(Vector2D *pos, Vector2D *vel, double angle, double angleVel, double inv_mass, int type, int id) {
    vector<Vector2D> *points = new vector<Vector2D>;
    vector<Line> *lines = new vector<Line>;

/* 1 - []
 * 2 - *
 * 3 - X
 * 4 - L
 * 5 - T
 * 6 - V
 * 7 - triangle
 * 8 - pentagon
 * 9 - hexagon
 * 10 - stick
 */
    switch (type) {
        case 1:
            points->push_back(*(new Vector2D(25, 25)));
            points->push_back(*(new Vector2D(-25, 25)));
            points->push_back(*(new Vector2D(-25, -25)));
            points->push_back(*(new Vector2D(25, -25)));
            break;
        case 2:
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 10)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 10)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 10)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 10)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 10)));

            for (int i = 0; i < points->size(); i++) {
                points->at(i).rotate(0.6283185 * i);
            }
            break;
        case 3:
            points->push_back(*(new Vector2D(-5, 25)));
            points->push_back(*(new Vector2D(5, 25)));
            points->push_back(*(new Vector2D(5, 5)));
            points->push_back(*(new Vector2D(25, 5)));
            points->push_back(*(new Vector2D(25, -5)));
            points->push_back(*(new Vector2D(5, -5)));
            points->push_back(*(new Vector2D(5, -25)));
            points->push_back(*(new Vector2D(-5, -25)));
            points->push_back(*(new Vector2D(-5, -5)));
            points->push_back(*(new Vector2D(-25, -5)));
            points->push_back(*(new Vector2D(-25, 5)));
            points->push_back(*(new Vector2D(-5, 5)));
            break;
        case 4:
            points->push_back(*(new Vector2D(-25, -25)));
            points->push_back(*(new Vector2D(-25, 25)));
            points->push_back(*(new Vector2D(-15, 25)));
            points->push_back(*(new Vector2D(-15, -15)));
            points->push_back(*(new Vector2D(0, -15)));
            points->push_back(*(new Vector2D(0, -25)));

            for (int i = 0; i < points->size(); i++) {
                points->at(i).add(new Vector2D(10, 11));
            }
            break;
        case 5:
            points->push_back(*(new Vector2D(-25, 25)));
            points->push_back(*(new Vector2D(25, 25)));
            points->push_back(*(new Vector2D(25, 15)));
            points->push_back(*(new Vector2D(5, 15)));
            points->push_back(*(new Vector2D(5, -25)));
            points->push_back(*(new Vector2D(-5, -25)));
            points->push_back(*(new Vector2D(-5, 15)));
            points->push_back(*(new Vector2D(-25, 15)));

            for (int i = 0; i < points->size(); i++) {
                points->at(i).add(new Vector2D(0, -13));
            }
            break;
        case 6:
            points->push_back(*(new Vector2D(-25, -25)));
            points->push_back(*(new Vector2D(-25, 25)));
            points->push_back(*(new Vector2D(-15, 25)));
            points->push_back(*(new Vector2D(-15, -15)));
            points->push_back(*(new Vector2D(25, -15)));
            points->push_back(*(new Vector2D(25, -25)));

            for (int i = 0; i < points->size(); i++) {
                points->at(i).add(new Vector2D(9, 9));
            }
            break;
        case 7:
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->at(1).rotate(2.0944);
            points->at(2).rotate(-2.0944);
            break;
        case 8:
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));

            for (int i = 0; i < points->size(); i++) {
                points->at(i).rotate(1.25664 * i);
            }
            break;
        case 9:
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));
            points->push_back(*(new Vector2D(0, 25)));

            for (int i = 0; i < points->size(); i++) {
                points->at(i).rotate(1.0472 * i);
            }
            break;
        case 10:
            points->push_back(*(new Vector2D(-5, 25)));
            points->push_back(*(new Vector2D(5, 25)));
            points->push_back(*(new Vector2D(5, -25)));
            points->push_back(*(new Vector2D(-5, -25)));
            break;
        default:
            cout << "Polygons must be created with a type between 1 and 10." << endl;
            exit(1);
    }

    // put lines in the vector of lines that connect all of the poitns that were just created
    for (int i = 0; i < points->size() - 1; i++) {
        lines->push_back(*(new Line(&(points->at(i)), &(points->at(i + 1)))));
    }
    lines->push_back(*(new Line(&(points->at(points->size() - 1)), &(points->at(0)))));

    // initialize all the lines to be in the right position and at the right angle
    for (int i = 0; i < lines->size(); i++) {
        lines->at(i).mult(1 / inv_mass);
        lines->at(i).rotate(angle);
        lines->at(i).add(pos);
    }

    initPolygon(pos, vel, angle, angleVel, inv_mass, lines, id);
}

Polygon::~Polygon() {
    delete lines;
}

// returns whether this Polygon is colliding with the given one
bool Polygon::isColliding(Polygon *other) {
    int len1 = abs(this->pos.x - other->pos.x);
    int len2 = abs(this->pos.y - other->pos.y);

    Vector2D thisSize = this->aabbMax.copy();
    thisSize.sub(&this->aabbMin);

    Vector2D otherSize = other->aabbMax.copy();
    otherSize.sub(&other->aabbMin);

    // AABB algorithm outlined in IEEE paper
    if (len1 <= (thisSize.x + otherSize.x) / 2 &&
        len2 <= (thisSize.y + otherSize.y) / 2) {

        // check if any of the lines are colliding with the other one's lines
        for (int i = 0; i < lines->size(); i++) {
            for (int j = 0; j < other->lines->size(); j++) {
                if (lines->at(i).isColliding(&(other->lines->at(j)))) return true;
            }
        }
    }

    return false;
}

// updates the AABB, position, angle, and lines of this Polygon, once per tick
void Polygon::update(Screen *s) {
    draw(s, false);

    pos.add(&vel);
    angle += angleVel;

    for (int i = 0; i < lines->size(); i++) {
        lines->at(i).sub(&pos);

        // line must be translated to the origin, because rotation occurs about the origin

        lines->at(i).rotate(angleVel);

        lines->at(i).add(&pos);
        lines->at(i).add(&vel);
    }

    // calculates the new AABB
    int xMax, yMax;
    int xMin = xMax = lines->at(0).p1.x;
    int yMin = yMax = lines->at(0).p1.y;

    for (int i = 1; i < lines->size(); i++) {
        Vector2D point = lines->at(i).p1;
        if (point.x < xMin) xMin = point.x;
        if (point.y < yMin) yMin = point.y;
        if (point.x > xMax) xMax = point.x;
        if (point.y > yMax) yMax = point.y;
    }
    aabbMin.x = xMin;
    aabbMin.y = yMin;
    aabbMax.x = xMax;
    aabbMax.y = yMax;
}

// draws this Polygon to the given screen
void Polygon::draw(Screen *s, bool isWhite) {
    pos.draw(s, isWhite);
    for (int i = 0; i < lines->size(); i++) {
        lines->at(i).draw(s, isWhite);
    }
}

// compares two Polygons by their ids
bool Polygon::operator<(const Polygon &p2) const {
    return this->id < p2.id;
}