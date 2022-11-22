#include "CollisionPair.h"
#include <iostream>
#include <stdlib.h>

// guarantees that the smaller Polygon will be in the first slot, for ordering purposes
CollisionPair::CollisionPair(Polygon *p1, Polygon *p2) {
    if (p1->id < p2->id) {
        this->p1 = p1;
        this->p2 = p2;
    }
    else{
        this->p1 = p2;
        this->p2 = p1;
    }
}

// compares two CollisionPairs by comparing the ids of their Polygons
bool CollisionPair::equals(CollisionPair *a, CollisionPair *b) {
    return a->p1->id == b->p1->id && a->p2->id == b->p2->id;
}

// compares two CollisionPairs by comparing their Polygons lexicographically
bool CollisionPair::operator<(const CollisionPair &cp2) const {
//    return this->p1 < cp2.p1 || this->p2 < cp2.p2;
//    return (this->p1 < cp2.p1 && this->p2 < cp2.p2) || (this->p1 < cp2.p2 && this->p2 < cp2.p1);
//    return this->intRep() < cp2.intRep();
    if(this->p1->id != cp2.p1->id){
        return this->p1->id < cp2.p1->id;
    }
    else{
        return this->p2->id < cp2.p2->id;
    }
}

// updates the Polygons' trajectories based on their past trajectories
void CollisionPair::handleCollision() {
    // currently not quite working

    Vector2D rv = p2->vel.copy();
    rv.sub(&p1->vel);

    Vector2D *normal;

    Vector2D n = p2->pos.copy();
    n.sub(&p1->pos);

    double ax_extent = abs(p1->aabbMax.x - p1->aabbMin.x) / 2;
    double bx_extent = abs(p2->aabbMax.x - p2->aabbMin.x) / 2;

    double x_overlap = ax_extent + bx_extent - abs(n.x);

    double ay_extent = abs(p1->aabbMax.y - p1->aabbMin.y) / 2;
    double by_extent = abs(p2->aabbMax.y - p2->aabbMin.y) / 2;

    double y_overlap = ay_extent + by_extent - abs(n.y);

    if (x_overlap > y_overlap) {
        if (n.x < 0)
            normal = new Vector2D(0, -1);
        else
            normal = new Vector2D(0, 1);
    } else {
        if (n.y < 0)
            normal = new Vector2D(-1, 0);
        else
            normal = new Vector2D(1, 0);
    }

    double velAlongNormal = rv.x * normal->x + rv.y * normal->y;

    if (velAlongNormal > 0){
        return;
    }

    double j = -2 * velAlongNormal;
    j /= p1->inv_mass + p2->inv_mass;

    normal->mult(j);

    Vector2D impulse1 = normal->copy();
    impulse1.mult(p1->inv_mass);

    Vector2D impulse2 = normal->copy();
    impulse2.mult(p2->inv_mass);

    p1->vel.sub(&impulse1);
    p2->vel.add(&impulse2);
}

