#include <cstdlib>
#include <param_config.hpp>

void ParamConfig::assign_int(const char *val, void *buffer)
{
    int *i = reinterpret_cast<int *>(buffer);
    *i = atoi(val);
}

void ParamConfig::assign_double(const char *val, void *buffer)
{
    double *d = reinterpret_cast<double *>(buffer);
    *d = atof(val);
}

void ParamConfig::assign_str(const char *val, void *buffer)
{
    strcpy(reinterpret_cast<char *>(buffer), val);
}

const std::vector<ParamConfig::AssignFunc>
ParamConfig::ASSIGN_FUNCS = {
    ParamConfig::assign_int,
    ParamConfig::assign_double,
    ParamConfig::assign_str
};

ParamConfig::ParamConfig(const char *key, ParamType type,
                         bool mandatory, void *buffer,
                         const CheckFunc& check)
{
    if(key[0] == '\0') {
        throw std::runtime_error("Invalid key");
    }

    if(type < PARAM_MIN || type > PARAM_MAX) {
        throw std::runtime_error("Invalid type");
    }

    if(!buffer) {
        throw std::runtime_error("Invalid buffer");
    }

    strcpy(key_, key);
    type_ = type;
    mandatory_ = mandatory;
    buffer_ = buffer;
    check_ = check;
}

bool ParamConfig::read_val(const char *v)
{
    ASSIGN_FUNCS[type_](v, buffer_);

    if(check_) {
        valid_ = check_(buffer_);
    } else {
        valid_ = true;
    }

    return valid_;
}

