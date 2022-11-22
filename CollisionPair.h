#ifndef PROJECT_COLLISIONPAIR_H
#define PROJECT_COLLISIONPAIR_H


#include "Polygon.h"

// represents a pair of Polygons that we know are colliding
class CollisionPair {
public:
    Polygon *p1;
    Polygon *p2;

    // guarantees that the smaller Polygon will be in the first slot, for ordering purposes
    CollisionPair(Polygon *, Polygon *);

    // compares two CollisionPairs by comparing the ids of their Polygons
    static bool equals(CollisionPair *, CollisionPair *);

    // compares two CollisionPairs by comparing their Polygons lexicographically
    bool operator<(const CollisionPair &p2) const;

    // updates the Polygons' trajectories based on their past trajectories
    void handleCollision();
};


#endif
