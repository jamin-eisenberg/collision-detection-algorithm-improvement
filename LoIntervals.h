#ifndef PROJECT_LOINTERVALS_H
#define PROJECT_LOINTERVALS_H

// represents an interval marker, the opening or closing of a shape interval
struct Interval {
    double *pos;
    bool isBegin;
    int shapeIndex;

    Interval();

    Interval(double *pos, bool isBegin, int shapeIndex);

    // compares intervals by comparing their positions
    bool operator<(const Interval &i2) const;
};

// represents a linked list of intervals
class LoIntervals {
public:
    // return whether the list is empty
    virtual bool empty() = 0;

    // returns the size of the list
    virtual int size() = 0;

    // returns the element at the given index
    virtual Interval *at(int index) = 0;

    // returns the list with the element at the given index removed
    virtual LoIntervals *erase(int index) = 0;
};

class MtLoInterval : public LoIntervals {
public:
    MtLoInterval();

    bool empty();

    int size();

    Interval *at(int index);

    LoIntervals *erase(int index);
};

class ConsoLoInterval : public LoIntervals {
private:
    Interval *first;
    LoIntervals *rest;
public:
    ConsoLoInterval(Interval *, LoIntervals *);

    bool empty();

    int size();

    Interval *at(int index);

    LoIntervals *erase(int index);
};

#endif
