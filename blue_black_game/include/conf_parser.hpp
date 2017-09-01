#ifndef __CONF_PARSER_HPP__
#define __CONF_PARSER_HPP__

#include <fstream>
#include <vector>
#include <functional>

struct ParamConfig {
    static const int KEY_LEN;

    char key[50];
    char type[2];
    bool mandatory;
    void *buffer;
    std::function<bool(ParamConfig *p)> check_func;
};

class ConfParser {
public:
    static constexpr int MAX_LINE_LEN = 1000;
    static constexpr int MAX_KEY_LEN = 200;
    static constexpr int MAX_VAL_LEN = 500;
    static std::vector<const char *> SUPPORTED_TYPES;

    static bool check_type(const char *type);

    ConfParser(std::vector<ParamConfig> *configs);

    virtual ~ConfParser();

    void read_config(const char *conf_file);

protected:
    char line_[MAX_LINE_LEN];
    char key_[MAX_KEY_LEN];
    char val_[MAX_VAL_LEN];
    std::vector<ParamConfig> *configs_;
    std::vector<bool> read_;

    bool next(std::ifstream& is);

    void reset();

    void check_configs();

    ParamConfig *find_config(const char *key);

    void assign_value(ParamConfig *cfg);

    void check_read();
};

#endif

