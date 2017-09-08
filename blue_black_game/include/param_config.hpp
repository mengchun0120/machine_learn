#ifndef __PARAM_CONFIG_HPP__
#define __PARAM_CONFIG_HPP__

#include <cstring>
#include <assign_func.hpp>

class ParamConfig {
public:
    static constexpr int KEY_LEN = 50;

    ParamConfig(const char *key, bool mandatory, void *buffer,
                AssignFunc assign_func);

    void read_val(const char *v)
    {
        (*assign_func_)(v, buffer_);
        read_ = true;
    }

    const bool match(const char *key) const
    {
        return strcmp(key_, key) == 0;
    }

    const char *key() const
    {
        return key_;
    }

    bool mandatory() const
    {
        return mandatory_;
    }

    const void *buffer() const
    {
        return buffer_;
    }

    const bool read() const
    {
        return read_;
    }

    void reset()
    {
        read_ = false;
    }

private:
    char key_[KEY_LEN];
    bool mandatory_;
    void *buffer_;
    bool read_;
    AssignFunc assign_func_;
};

#endif

