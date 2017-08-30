#include <cassert>
#include <cstring>
#include <utils.hpp>
#include <cstdio>

int main()
{
    char s[] = "";
    strtrim(s);
    assert(strcmp(s, "") == 0);

    char s1[] = " 1234 6789 ";
    strtrim(s1);
    assert(strcmp(s1, "1234 6789") == 0);

    char s2[] = "123456    ";
    strtrim(s2);
    assert(strcmp(s2, "123456") == 0);

    char s3[] = "   123";
    strtrim(s3);
    assert(strcmp(s3, "123") == 0);

    char s4[] = "     \t  ";
    strtrim(s4);
    assert(strcmp(s4, "") == 0);
}

