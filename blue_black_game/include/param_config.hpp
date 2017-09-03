#ifndef __PARAM_CONFIG_HPP__
#define __PARAM_CONFIG_HPP__

#include <functional>
#include <utility>
#include <cstring>
#include <vector>
#include <check_func.hpp>

class ParamConfig {
public:
    enum ParamType {
        PARAM_MIN = 0,
        INT_PARAM = PARAM_MIN,
        DOUBLE_PARAM,
        STRING_PARAM,
        PARAM_MAX = STRING_PARAM
    };

    typedef std::function<void(const char *, void *)>
            AssignFunc;

    static void assign_int(const char *val, void *buffer);

    static void assign_double(const char *val, void *buffer);

    static void assign_str(const char *val, void *buffer);

    static const std::vector<AssignFunc> ASSIGN_FUNCS;

    static constexpr int KEY_LEN = 50;

    ParamConfig(const char *key, ParamType type,
                bool mandatory, void *buffer,
                const CheckFunc& check=nullptr);

    bool read_val(const char *v);

    const bool match(const char *key) const
    {
        return strcmp(key_, key) == 0;
    }

    const char *key() const
    {
        return key_;
    }

    ParamType type() const
    {
        return type_;
    }

    bool mandatory() const
    {
        return mandatory_;
    }

    const void *buffer() const
    {
        return buffer_;
    }

    const bool valid() const
    {
        return valid_;
    }

    void reset()
    {
        valid_ = false;
    }

private:
    char key_[KEY_LEN];
    ParamType type_;
    bool mandatory_;
    void *buffer_;
    CheckFunc check_;
    bool valid_;
};

#endif

