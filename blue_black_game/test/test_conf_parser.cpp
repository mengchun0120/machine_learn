#include <cassert>
#include <cstring>
#include <iostream>
#include <conf_parser.hpp>
#include <assign_func.hpp>

int main()
{
    /* intkey = 1234
       strkey= str
       doublekey= 123.5
       probkey=0.8 */

    int i = 0;
    char s[10];
    double d = 0.0;
    double p = 0.0;

    ParamConfig configs[] = {
        ParamConfig("intkey", true,
                    reinterpret_cast<void *>(&i),
                    assign_int),
        ParamConfig("strkey", true,
                    reinterpret_cast<void *>(s),
                    assign_str),
        ParamConfig("doublekey", true,
                    reinterpret_cast<void *>(&d),
                    assign_double),
        ParamConfig("probkey", true,
                    reinterpret_cast<void *>(&p),
                    assign_double)
    };

    int count = static_cast<int>(sizeof(configs)/
                                 sizeof(ParamConfig));

    ConfParser parser(configs, count);
    parser.read_config("test/test.conf");

    assert(i == 1234);
    assert(strcmp(s, "str") == 0);
    assert(d == 123.5);
    assert(p == 0.8);
}

