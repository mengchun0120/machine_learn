#include <cassert>
#include <cstring>
#include <iostream>
#include <conf_parser.hpp>

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

    std::vector<ParamConfig> configs = {
        ParamConfig("intkey", ParamConfig::INT_PARAM,
                    true, reinterpret_cast<void *>(&i),
                    lbound_check<int, false>(0)),

        ParamConfig("strkey", ParamConfig::STRING_PARAM,
                    true, reinterpret_cast<void *>(s)),

        ParamConfig("doublekey", ParamConfig::DOUBLE_PARAM,
                    true, reinterpret_cast<void *>(&d),
                    lbound_check<double, false>(0.0)),

        ParamConfig("probkey", ParamConfig::DOUBLE_PARAM, 
                    true, reinterpret_cast<void *>(&p),
                    lbound_check<double, false>(0.0) &&
                    ubound_check<double, false>(1.0))
    };

    ConfParser parser(&configs);
    parser.read_config("test/test.conf");

    assert(i == 1234);
    assert(strcmp(s, "str") == 0);
    assert(d == 123.5);
    assert(p == 0.8);
}

