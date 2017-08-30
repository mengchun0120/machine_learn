#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <utils.hpp>
#include <conf_parser.hpp>

const int ConfParser::MAX_LINE_LEN = 1000;
const int ConfParser::MAX_KEY_LEN = 200;
const int ConfParser::MAX_VAL_LEN = 800;

ConfParser::ConfParser(const char *conf_file):
    is_(conf_file)
{
    line_ = new char[MAX_LINE_LEN];
    key_ = new char[MAX_KEY_LEN];
    val_ = new char[MAX_VAL_LEN];
}

ConfParser::~ConfParser()
{
    is_.close();
    delete[] line_;
    delete[] key_;
    delete[] val_;
}

bool ConfParser::next()
{
    if(!is_.good()) return false;

    is_.getline(line_, MAX_LINE_LEN);

    char *eq = strchr(line_, '=');

    if(eq == nullptr) {
        throw std::runtime_error("Invalid conf file");
    }

    *eq = '\0';
    strcpy(key_, line_);
    strtrim(key_);
    if(strcmp(key_, "") == 0) {
        throw std::runtime_error("Invalid conf file");
    }

    strcpy(val_, eq + 1);
    strtrim(val_);
    if(strcmp(val_, "") == 0) {
        throw std::runtime_error("Invalid conf file");
    }

    return true;
}


