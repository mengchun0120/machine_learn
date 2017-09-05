#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <utils.hpp>
#include <conf_parser.hpp>

ConfParser::ConfParser(std::vector<ParamConfig> *configs)
{
    if(!configs) {
        throw std::runtime_error("configs is null");
    }
    configs_ = configs;
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
            printf("unrecognized: %s\n", key_);
            throw std::runtime_error("Unrecognized key");
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
    for(auto it = configs_->begin();
        it != configs_->end(); ++it) {

        it->reset();
    }
}

ParamConfig *ConfParser::find_config(const char *key)
{
    ParamConfig *cfg = nullptr;

    for(auto it = configs_->begin();
        it != configs_->end(); ++it) {

        if(it->match(key)) {
            cfg = &(*it);
            break;
        }
    }

    return cfg;
}

void ConfParser::check_read()
{
    for(auto it = configs_->begin();
        it != configs_->end(); ++it) {

        if(it->mandatory() && !it->valid()) {
            throw std::runtime_error("Mandatory not read");
        }
    }
}
