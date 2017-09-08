#ifndef __CONF_PARSER_HPP__
#define __CONF_PARSER_HPP__

#include <fstream>
#include <param_config.hpp>

class ConfParser {
public:
    static constexpr int MAX_LINE_LEN = 1000;
    static constexpr int MAX_KEY_LEN = 200;
    static constexpr int MAX_VAL_LEN = 500;

    ConfParser(ParamConfig *configs, int count);

    virtual ~ConfParser()
    {}

    void set_config(ParamConfig *configs, int count);

    void read_config(const char *conf_file);

protected:
    char line_[MAX_LINE_LEN];
    char key_[MAX_KEY_LEN];
    char val_[MAX_VAL_LEN];
    ParamConfig *configs_;
    int count_;

    bool next(std::ifstream& is);

    void reset();

    ParamConfig *find_config(const char *key);

    void assign_value(ParamConfig *cfg);

    void check_read();
};

#endif

