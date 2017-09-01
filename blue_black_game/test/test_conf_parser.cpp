#include <cassert>
#include <cstring>
#include <iostream>
#include <conf_parser.hpp>

int main()
{
    const char *conf_file = "test/test.conf";
    const char *keys[] = {"key1", "key2", "key_34"};
    const char *vals[] = {"val1", "val2", "val_34"};
    ConfParser parser(conf_file);
    int i;

    for(i = 0; parser.next(); ++i) {
        assert(strcmp(keys[i], parser.key()) == 0);
        assert(strcmp(vals[i], parser.val()) == 0);
    }

    assert(i == 3);
}

