#include <cstring>
#include <param_config.hpp>

ParamConfig::ParamConfig(const char *key, bool mandatory,
                         void *buffer, AssignFunc assign_func)
{
    if(key[0] == '\0') {
        throw std::runtime_error("ParamConfig::ParamConfig "\
                                 "Invalid key");
    }

    if(!buffer) {
        throw std::runtime_error("ParamConfig::ParamConfig "\
                                 "Invalid buffer");
    }

    strcpy(key_, key);
    mandatory_ = mandatory;
    buffer_ = buffer;
    assign_func_ = assign_func;
}


