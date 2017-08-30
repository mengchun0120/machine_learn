#ifndef __CONF_PARSER_HPP__
#define __CONF_PARSER_HPP__

#include <fstream>

class ConfParser {
public:
    static const int MAX_LINE_LEN;
    static const int MAX_KEY_LEN;
    static const int MAX_VAL_LEN;

    ConfParser(const char *conf_file);

    virtual ~ConfParser();

    bool next();

    const char *key() const
    {
        return key_;
    }

    const char *val() const
    {
        return val_;
    }

protected:
    char *line_;
    char *key_;
    char *val_;
    std::ifstream is_;
};

#endif

