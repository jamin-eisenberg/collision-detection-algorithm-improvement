#include <iostream>
//#include <stdlib.h>
#include <ctime>
#include <set>
#include <algorithm>
#include "DE1SoCfpga.h"
#include "Screen.h"
#include "Keypad.h"
#include "Polygon.h"
#include "Speaker.h"
#include "CollisionPair.h"
#include "LoIntervals.h"

/* TODO
   make presentation
   make video
   submit
 */

// Cited IEEE paper for collision detection: https://ieeexplore-ieee-org.ezproxy.neu.edu/document/5363537
// Collision handling: http://www.kuffner.org/james/software/dynamics/mirtich/mirtichThesis.pdf

// returns the polygon that the given polygon is colliding with
Polygon *collidingWithAnything(Polygon *shape, Polygon *shapes, int numShapes) {
    for (int i = 0; i < numShapes; i++) {
        if (&shapes[i] == shape) continue;
        if (shape->isColliding(&shapes[i])) return &shapes[i];
    }

    return NULL;
}

// makes a new polygon with random attributes besides mass and type
Polygon *makeRandomPolygon(double mass, int type, Screen *s, int id) {
    Polygon *p = new Polygon(
            new Vector2D(((double) 320 * rand() / RAND_MAX), ((double) 240 * rand() / RAND_MAX)),
            new Vector2D(((double) 2 * rand() / RAND_MAX) - 1, ((double) 2 * rand() / RAND_MAX) - 1),
            ((double) 6.28 * rand() / RAND_MAX), ((double) 0.05 * rand() / RAND_MAX), 1 / mass, type, id);
//    Polygon *p = new Polygon(
//            new Vector2D(((double) 320 * rand() / RAND_MAX), ((double) 240 * rand() / RAND_MAX)),
//            new Vector2D(((double) 2 * rand() / RAND_MAX) - 1, ((double) 2 * rand() / RAND_MAX) - 1),
//            0, 0, 1 / mass, type, id);
    p->update(s); // give it a proper AABB
    return p;
}

// compares two CollisionPairs (overriding operator< wasn't working for some reason)
struct cpCmp {
    bool operator()(const CollisionPair *a, const CollisionPair *b) {
        if (a->p1->id != b->p1->id) {
            return a->p1->id < b->p1->id;
        } else {
            return a->p2->id < b->p2->id;
        }
    }
};

// the core of the optimized algorithm, returns a set of CollisionPairs that are colliding in the given vector of intervals
set<CollisionPair *, cpCmp> *getPossibleCollided(vector<Interval> *collisionIntervals, Polygon *shapes) {
    set<CollisionPair *, cpCmp> *possibleCollided = new set<CollisionPair *, cpCmp>();

    LoIntervals *active = new MtLoInterval();

    sort(collisionIntervals->begin(), collisionIntervals->end());

    for (int i = 0; i < collisionIntervals->size(); i++) {
        if (collisionIntervals->at(i).isBegin) {
            if (!active->empty()) {
                for (int j = 0; j < active->size(); j++) {
                    possibleCollided->insert(new CollisionPair(&shapes[collisionIntervals->at(i).shapeIndex],
                                                               &shapes[active->at(j)->shapeIndex]));
                }
            }

            active = new ConsoLoInterval(&collisionIntervals->at(i), active);
        } else {
            int indexOfPair = -1;

            for (int j = 0; j < active->size(); j++) {
                if (collisionIntervals->at(i).shapeIndex == active->at(j)->shapeIndex) {
                    indexOfPair = j;
                    break;
                }
            }

            active = active->erase(indexOfPair);
        }
    }

    return possibleCollided;
}

// initializes the edges' lines
void initEdges(vector<Line> *edge1, vector<Line> *edge2, vector<Line> *edge3, vector<Line> *edge4) {
    edge1->push_back(Line(new Vector2D(-1, 0), new Vector2D(-1, 240 - 1)));
    edge1->push_back(Line(new Vector2D(-20, 0), new Vector2D(-20, 240 - 1)));
    edge1->push_back(Line(new Vector2D(-1, 0), new Vector2D(-20, 0)));
    edge1->push_back(Line(new Vector2D(-1, 240 - 1), new Vector2D(-20, 240 - 1)));

    edge2->push_back(Line(new Vector2D(0, 241), new Vector2D(320 - 1, 241)));
    edge2->push_back(Line(new Vector2D(0, 260), new Vector2D(320 - 1, 260)));
    edge2->push_back(Line(new Vector2D(0, 241), new Vector2D(0, 260)));
    edge2->push_back(Line(new Vector2D(320 - 1, 241), new Vector2D(320 - 1, 260)));

    edge3->push_back(Line(new Vector2D(321, 240), new Vector2D(321, 1)));
    edge3->push_back(Line(new Vector2D(340, 240), new Vector2D(340, 1)));
    edge3->push_back(Line(new Vector2D(321, 240), new Vector2D(340, 240)));
    edge3->push_back(Line(new Vector2D(321, 1), new Vector2D(340, 1)));

    edge4->push_back(Line(new Vector2D(320, -1), new Vector2D(1, -1)));
    edge4->push_back(Line(new Vector2D(320, -20), new Vector2D(1, -20)));
    edge4->push_back(Line(new Vector2D(320, -1), new Vector2D(320, -20)));
    edge4->push_back(Line(new Vector2D(1, -1), new Vector2D(1, -20)));
}

int main() {
    cout << "Starting..." << endl;

    // how many shapes can be processed
    const int MAX_SHAPES = 250;

    DE1SoCfpga *fpga = new DE1SoCfpga();
    Screen *screen = new Screen(fpga);
    Keypad *keypad = new Keypad();

    // keep track of x and y intervals
    vector<Interval> *xCollisionIntervals = new vector<Interval>();
    vector<Interval> *yCollisionIntervals = new vector<Interval>();

    // for finding the average time of calculation per tick
    double totalTime = 0;
    int ticksRecorded = 0;

    srand(time(NULL));

    Speaker *speaker = new Speaker();

    vector<Line> *edge1 = new vector<Line>();
    vector<Line> *edge2 = new vector<Line>();
    vector<Line> *edge3 = new vector<Line>();
    vector<Line> *edge4 = new vector<Line>();

    initEdges(edge1, edge2, edge3, edge4);

    // edges are Polygons as well, and we always start with them
    Polygon shapes[MAX_SHAPES] = {
            Polygon(new Vector2D(-10, 120), new Vector2D(), 0, 0, 0, edge1, 0),
            Polygon(new Vector2D(160, 250), new Vector2D(), 0, 0, 0, edge2, 1),
            Polygon(new Vector2D(330, 120), new Vector2D(), 0, 0, 0, edge3, 2),
            Polygon(new Vector2D(160, -10), new Vector2D(), 0, 0, 0, edge4, 3)
    };
    int numShapes = 4;

    // add the edges' AABBs to the intervals
    for (int i = 0; i < numShapes; i++) {
        xCollisionIntervals->push_back(Interval(&shapes[i].aabbMin.x, true, i));
        xCollisionIntervals->push_back(Interval(&shapes[i].aabbMax.x, false, i));
        yCollisionIntervals->push_back(Interval(&shapes[i].aabbMin.y, true, i));
        yCollisionIntervals->push_back(Interval(&shapes[i].aabbMax.y, false, i));
        shapes[i].update(screen);
    }

    double mass = 1;
    int keyPressed;
    int oldKeyPressed = -1;

    screen->clear();

    keyPressed = keypad->read();
    while (keyPressed != 16) { // Pressing 'D' ends the simulation
        if (oldKeyPressed != keyPressed && keyPressed != 0) { // if a new key is being pressed
            if (keyPressed >= 1 && keyPressed <= 10) { // if it's an insertion key
                int numInsertionTries = 0;

                do { // try to insert a new random polygon
                    shapes[numShapes] = *makeRandomPolygon(mass, keyPressed, screen, numShapes);
                    numInsertionTries++;
                } while (numInsertionTries < 100 && collidingWithAnything(&shapes[numShapes], shapes, numShapes + 1));
                // if the inserted polygon is colliding, try again up to 100 times

                // if we couldn't insert it
                if (numInsertionTries >= 100) {
                    cout << "Screen full. Insertion failed after " << numInsertionTries << "tries." << endl;
                } else {
                    cout << "New type " << keyPressed << " shape inserted with mass " << mass << endl;
                    // add the new shape's AABB to the intervals
                    xCollisionIntervals->push_back(Interval(&shapes[numShapes].aabbMin.x, true, numShapes));
                    xCollisionIntervals->push_back(Interval(&shapes[numShapes].aabbMax.x, false, numShapes));
                    yCollisionIntervals->push_back(Interval(&shapes[numShapes].aabbMin.y, true, numShapes));
                    yCollisionIntervals->push_back(Interval(&shapes[numShapes].aabbMax.y, false, numShapes));
                    numShapes++;
                }
            } else if (keyPressed == 11) { // '9' to decrease mass
                mass *= 0.9;
                cout << "Mass decreased to " << mass << endl;
            } else if (keyPressed == 12) { // 'C' to increase mass
                mass *= 1.11111;
                cout << "Mass increased to " << mass << endl;
            }
        }

        // update and draw every shape
        for (int i = 0; i < numShapes; i++) {
            shapes[i].update(screen);
            shapes[i].draw(screen, true);
        }

        clock_t start = clock();
        // start timing

        // IMPROVED ALGORITHM:
        set<CollisionPair *, cpCmp> *possibleXCollided = getPossibleCollided(xCollisionIntervals, shapes);
        set<CollisionPair *, cpCmp> *possibleYCollided = getPossibleCollided(yCollisionIntervals, shapes);

        vector<CollisionPair *> possibleCollided;

        set_intersection(possibleXCollided->begin(), possibleXCollided->end(), possibleYCollided->begin(),
                         possibleYCollided->end(), back_inserter(possibleCollided), cpCmp());

        vector<CollisionPair *> *collided = new vector<CollisionPair *>();

        for (int i = 0; i < possibleCollided.size(); i++) {
            CollisionPair *cp = possibleCollided.at(i);
            if (cp->p1->isColliding(cp->p2)) {
                collided->push_back(cp);
            }
        }

        // ORIGINAL ALGORITHM:
//        for (int i = 0; i < numShapes; i++) {
//            for (int j = 0; j < numShapes; j++) {
//                if (&shapes[i] == &shapes[j]) continue;
//                if (shapes[i].isColliding(&shapes[j])) collided->push_back(new CollisionPair(&shapes[i], &shapes[j]));
//            }
//        }

        // remove duplicate CollisionPairs
        sort(collided->begin(), collided->end());
        collided->erase(unique(collided->begin(), collided->end(), CollisionPair::equals), collided->end());


        vector<CollisionPair *> *newCollided = new vector<CollisionPair *>();
        for (int i = 0; i < collided->size(); i++) {
            newCollided->push_back(collided->at(i));
            newCollided->push_back(new CollisionPair(collided->at(i)->p2, collided->at(i)->p1));
        }

        // stop timing
        clock_t end = clock();

        // we've now timed another tick
        totalTime += double(end - start) / CLOCKS_PER_SEC;
        ticksRecorded++;

        if (!collided->empty()) speaker->playTone(0.016, 880); // play a short beep if a collision occurred


        // handle all collisions
        for (int i = 0; i < collided->size(); i++) {
            collided->at(i)->handleCollision();
        }

        // write to the screen
        screen->update(fpga);

        // get the new keypress
        oldKeyPressed = keyPressed;
        keyPressed = keypad->read();
    }

    cout << "Average collision calculation time: " << totalTime / ticksRecorded << endl;

    screen->clear();

    delete speaker;
    delete keypad;
    delete screen;
    delete fpga;

    cout << "Ending" << endl;

    exit(0);
}


