#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <utils.hpp>
#include <conf_parser.hpp>

std::vector<const char *> ConfParser::SUP_TYPES{"c", "d", "i"};

bool ConfParser::check_type(const char *type)
{
    bool valid = false;

    for(unsigned int i = 0; i < SUP_TYPES.size(); ++i)    {
        if(strcmp(SUP_TYPES[i], type) == 0) {
            valid = true;
            break;
        }
    }

    return valid;
}


ConfParser::ConfParser(std::vector<ParamConfig> *configs):
    configs_(configs)
{
    if(!configs_) {
        throw std::runtime_error("configs is null");
    }

    check_configs();
    read_.resize(configs_->size());
}

ConfParser::~ConfParser()
{
}

void ConfParser::read_config(const char *conf_file)
{
    reset();

    std::ifstream is(conf_file);
    ParamConfig *cfg;

    for(int i = 0; next(is); ++i) {
        cfg = find_config(key_);
        if(!cfg) {
            throw std::runtime_error("Unrecognized key");
        }

        assign_value(cfg);

        if(cfg->check) {
            if(!cfg->check(cfg)) {
                throw std::runtime_error("Invalid param");
            }
        }

        read_[i] = true;
    }

    check_read();
}

bool ConfParser::next(std::ifstream& is)
{
    if(!is.good()) return false;

    do {
        is.getline(line_, MAX_LINE_LEN);
        strtrim(line_);
        if(line_[0] != '\0') break;
    } while(is.good());

    if(!is.good()) return false;

    char *eq = strchr(line_, '=');

    if(eq == nullptr) {
        throw std::runtime_error("No = in the line");
    }

    *eq = '\0';
    strcpy(key_, line_);
    strtrim(key_);
    if(strcmp(key_, "") == 0) {
        throw std::runtime_error("Key is empty");
    }

    strcpy(val_, eq + 1);
    strtrim(val_);
    if(strcmp(val_, "") == 0) {
        throw std::runtime_error("Value is empty");
    }

    return true;
}

void ConfParser::reset()
{
    std::fill(read_.begin(), read_.end(), false);
}


void ConfParser::check_configs()
{
    if(configs_->size() == 0) {
        throw std::runtime_error("Configs is emptry");
    }

    for(auto it = configs_->begin(); it != configs_->end(); ++it) {
        if(it->key[0] == '\0') {
            throw std::runtime_error("Key in configs is empty");
        }

        if(it->type[0] == '\0' || !check_type(it->type)) {
            throw std::runtime_error("Type in configs is invalid");
        }

        if(it->buffer == nullptr) {
            throw std::runtime_error("Buffer in configs is null");
        }
    }
}

ParamConfig *ConfParser::find_config(const char *key)
{
    ParamConfig *cfg = nullptr;

    for(auto it = configs_->begin(); it != configs_->end(); ++it) {
        if(strcmp(it->key, key) == 0) {
            cfg = &(*it);
            break;
        }
    }

    return cfg;
}

void ConfParser::assign_value(ParamConfig *cfg)
{
    if(strcmp(cfg->type, "c") == 0) {
        strcpy(reinterpret_cast<char *>(cfg->buffer), val_);
    } else if(strcmp(cfg->type, "d") == 0) {
        *reinterpret_cast<double *>(cfg->buffer) = atof(val_);
    } else if(strcmp(cfg->type, "i") == 0) {
        *reinterpret_cast<int *>(cfg->buffer) = atoi(val_);
    }
}

void ConfParser::check_read()
{
    for(unsigned int i = 0; i < read_.size(); ++i) {
        if((*configs_)[i].mandatory && !read_[i]) {
            throw std::runtime_error("Mandatory param not read");
        }
    }
}
