#include <cstdlib>
#include <cstring>
#include <assign_func.hpp>
#include <point.hpp>
#include <utils.hpp>

void assign_int(const char *val, void *buffer)
{
    int *i = reinterpret_cast<int *>(buffer);
    *i = atoi(val);
}

void assign_double(const char *val, void *buffer)
{
    double *d = reinterpret_cast<double *>(buffer);
    *d = atof(val);
}

void assign_str(const char *val, void *buffer)
{
    strcpy(reinterpret_cast<char *>(buffer), val);
}

void assign_point(const char *val, void *buffer)
{
    char s[strlen(val)+1];
    strcpy(s, val);

    char *p = strchr(s, ',');
    if(!p) {
        throw std::runtime_error("Point::from_str " \
                                 "Invalid str");
    }

    *p = '\0';
    ++p;

    strtrim(s);
    if(*s == '\0') {
        throw std::runtime_error("Point::from_str " \
                                 "Invalid x");
    }

    strtrim(p);
    if(*p == '\0') {
        throw std::runtime_error("Point::from_str " \
                                 "Invalid y");
    }

    Point *pt = reinterpret_cast<Point *>(buffer);

    pt->x = atoi(s);
    pt->y = atoi(p);
}

