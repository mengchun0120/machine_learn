#include <cstring>
#include <utils.hpp>
#include <cstdio>

void strtrim(char *s)
{
    char *p = s;

    while(*p == ' ' || *p == '\t') {
        ++p;
    }

    if(*p == '\0') {
        *s = '\0';
        return;
    }

    char *q = p + strlen(p) - 1;
    while(*q == ' ' || *q == '\t') {
        q--;
    }
    *(q+1) = '\0';

    if(p != s) {
        while(*p != '\0') {
            *(s++) = *(p++);
        }
        *s = '\0';
    }
}

