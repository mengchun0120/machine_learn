#ifndef __CONF_PARSER_HPP__
#define __CONF_PARSER_HPP__

#include <fstream>
#include <vector>
#include <stdexcept>
#include <functional>

typedef std::function<bool(const void *)> CheckFunc;

struct ParamConfig {
    static const int KEY_LEN;

    char key[50];
    char type[2];
    bool mandatory;
    void *buffer;
    CheckFunc check;
};

template<typename T, bool inclusive=true>
CheckFunc lbound_check(const T& lbound)
{
    auto f = [&](const void *buffer)->bool
    {
        const T *v = reinterpret_cast<const T*>(buffer);
        return inclusive ? (*v >= lbound) : (*v > lbound);
    };
    return CheckFunc(f);
}

template<typename T, bool inclusive=true>
CheckFunc ubound_check(const T& ubound)
{
    auto f = [&](const void *buffer)->bool
    {
        const T *v = reinterpret_cast<const T*>(buffer);
        return inclusive ? (*v <= ubound) : (*v < ubound);
    };
    return CheckFunc(f);
}

CheckFunc operator&&(const CheckFunc& f1, const CheckFunc& f2);
{
    auto f = [&](const void *buffer)->bool
    {
        return f1(buffer) && f2(buffer);
    };

    return f;
}

CheckFunc operator||(const CheckFunc& f1, const CheckFunc& f2);
{
    auto f = [&](const void *buffer)->bool
    {
        return f1(buffer) || f2(buffer);
    };

    return f;
}

class ConfParser {
public:
    static constexpr int MAX_LINE_LEN = 1000;
    static constexpr int MAX_KEY_LEN = 200;
    static constexpr int MAX_VAL_LEN = 500;
    static std::vector<const char *> SUP_TYPES;

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

