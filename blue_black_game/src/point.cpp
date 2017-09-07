#include <point.hpp>
#include <assign_func.hpp>

Point::Point(const char *str)
{
    assign_point(str, reinterpret_cast<void *>(this));
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
    os << p.x << ',' << p.y;
    return os;
}


