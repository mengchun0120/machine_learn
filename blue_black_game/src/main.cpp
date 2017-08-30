#include <utils.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    char s[] = "";
    strtrim(s);
    std::cout << "s=\"" << s << "\"" << std::endl;

    char s1[] = " 1234 6789 ";
    strtrim(s1);
    std::cout << "s1=\"" << s1 << "\"" << std::endl;

    char s2[] = "123456    ";
    strtrim(s2);
    std::cout << "s2=\"" << s2 << "\"" << std::endl;

    char s3[] = "   123";
    strtrim(s3);
    std::cout << "s3=\"" << s3 << "\"" << std::endl;
}

