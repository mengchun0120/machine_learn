#ifndef __CONF_PARSER_HPP__
#define __CONF_PARSER_HPP__

#include <fstream>
#include <vector>
#include <check_func.hpp>
#include <param_config.hpp>

class ConfParser {
public:
    static constexpr int MAX_LINE_LEN = 1000;
    static constexpr int MAX_KEY_LEN = 200;
    static constexpr int MAX_VAL_LEN = 500;

    ConfParser(std::vector<ParamConfig> *configs);

    virtual ~ConfParser();

    void read_config(const char *conf_file);

protected:
    char line_[MAX_LINE_LEN];
    char key_[MAX_KEY_LEN];
    char val_[MAX_VAL_LEN];
    std::vector<ParamConfig> *configs_;

    bool next(std::ifstream& is);

    void reset();

    ParamConfig *find_config(const char *key);

    void assign_value(ParamConfig *cfg);

    void check_read();
};

#endif

