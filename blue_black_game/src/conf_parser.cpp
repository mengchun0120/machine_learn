#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <utils.hpp>
#include <conf_parser.hpp>

ConfParser::ConfParser(ParamConfig *configs, int count)
{
    set_config(configs, count);
}

void ConfParser::set_config(ParamConfig *configs, int count)
{
     if(!configs) {
        throw std::runtime_error("ConfParser::ConfParser "\
                                 "configs is null");
    }

    if(count <= 0) {
         throw std::runtime_error("ConfParser::ConfParser "\
                                  "count is invalid");
    }

    configs_ = configs;
    count_ = count;
}

void ConfParser::read_config(const char *conf_file)
{
    reset();

    std::ifstream is(conf_file);
    ParamConfig *cfg;

    for(int i = 0; next(is); ++i) {
        cfg = find_config(key_);
        if(!cfg) {
            throw std::runtime_error("ConfParser::"\
                        "read_config Unrecognized key");
        }

        cfg->read_val(val_);
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
    ParamConfig *c = configs_;

    for(int i = 0; i < count_; ++i, ++c) {
        c->reset();
    }
}

ParamConfig *ConfParser::find_config(const char *key)
{
    ParamConfig *ret = nullptr;
    ParamConfig *c = configs_;

    for(int i = 0; i < count_; ++i, ++c) {
        if(c->match(key)) {
            ret = c;
            break;
        }
    }

    return ret;
}

void ConfParser::check_read()
{
    ParamConfig *c = configs_;

    for(int i = 0; i < count_; ++i, ++c) {
        if(c->mandatory() && !c->read()) {
            throw std::runtime_error("ConfParser::check_read "\
                                     "mandatory not read");
        }
    }
}

