#ifndef __POINT_HPP__
#define __POINT_HPP__

#include <iostream>

struct Point {
    Point(const char *str);

    Point(int x=0, int y=0)
    {
        this->x = x;
        this->y = y;
    }

    Point(const Point& p):
        x(p.x), y(p.y)
    {

    }

    Point& from_str(const char *str);

    Point& operator=(const Point& p) const
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    bool operator==(const Point& p) const
    {
        return x == p.x && y == p.y;
    }

    int x;
    int y;
};

std::ostream& operator<<(std::ostream& os, const Point& p);

#endif

