#include "LoIntervals.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

Interval::Interval() = default;

Interval::Interval(double *pos, bool isBegin, int shapeIndex) {
    this->pos = pos;
    this->isBegin = isBegin;
    this->shapeIndex = shapeIndex;
}

// compares intervals by comparing their positions
bool Interval::operator<(const Interval &i2) const {
    return *this->pos < *i2.pos;
}

MtLoInterval::MtLoInterval() = default;

// return whether the list is empty
bool MtLoInterval::empty() {
    return true;
}

// returns the size of the list
int MtLoInterval::size() {
    return 0;
}

// returns the element at the given index
Interval *MtLoInterval::at(int index) {
    cout << "Error: index out of bounds." << endl;
    exit(1);
}

// returns the list with the element at the given index removed
LoIntervals *MtLoInterval::erase(int index) {
    cout << "Error: index out of bounds." << endl;
    exit(1);
}


ConsoLoInterval::ConsoLoInterval(Interval *first, LoIntervals *rest) {
    this->first = first;
    this->rest = rest;
}

// return whether the list is empty
bool ConsoLoInterval::empty() {
    return false;
}

// returns the size of the list
int ConsoLoInterval::size() {
    return 1 + this->rest->size();
}

// returns the element at the given index
Interval *ConsoLoInterval::at(int index) {
    if (index == 0)
        return first;
    return this->rest->at(index - 1);
}

// returns the list with the element at the given index removed
LoIntervals *ConsoLoInterval::erase(int index) {
    if (index == 0)
        return rest;
    return new ConsoLoInterval(this->first, this->rest->erase(index - 1));
}