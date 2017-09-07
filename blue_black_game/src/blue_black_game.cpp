#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <blue_black_game.hpp>
#include <conf_parser.hpp>
#include <utils.hpp>

BlueBlackGame::Config::Config(const char *conf_file)
{
    init(conf_file);
}

void BlueBlackGame::Config::init(const char *conf_file)
{
    ParamConfig cfgs[] = {
        ParamConfig("width", true,
                    reinterpret_cast<void *>(&width),
                    assign_int),
        ParamConfig("height", true,
                    reinterpret_cast<void *>(&board_height),
                    assign_int),
        ParamConfig("blue_pos", true,
                    reinterpret_cast<void *>(&blue_pos),
                    assign_point),
        ParamConfig("hole_pos", true,
                    reinterpret_cast<void *>(&hole_pos),
                    assign_point),
        ParamConfig("start_pos", true,
                    reinterpret_cast<void *>(&start_pos),
                    assign_point),
        ParamConfig("wind_prob", true,
                    reinterpret_cast<void *>(&wind_prob),
                    assign_double),
        ParamConfig("wind_direct", true,
                    reinterpret_cast<void *>(&wind_direct),
                    assign_double)
    };

    ConfParser parser(&cfgs);
    parser.read_config(conf_file);

    check();
}

void BlueBlackGame::Config::check()
{
    if(width < MIN_WIDTH) {
        throw std::runtime_error("BlueBlackGame::Config::"\
                        "check width too small");
    }

    if(height < MIN_HEIGHT) {
        throw std::runtime_error("BlueBlackGame::Config::"\
                        "check height too small");
    }

    if(!check_point(blue_pos)) {
        throw std::runtime_error("BlueBlackGame::Config::"\
                        "check blue_pos is invalid");
    }

    if(!check_point(hole_pos)) {
        throw std::runtime_error("BlueBlackGame::Config::"\
                        "check hole_pos is invalid");
    }

    if(!check_point(start_pos)) {
        throw std::runtime_error("BlueBlackGame::Config::"\
                        "check start_pos is invalid");
    }

    if(!BlueBlackGame::valid_direct_str(wind_direct)) {
        throw std::runtime_error("BlueBlackGame::Config::"\
                        "check wind_direct is invalid");
    }

    if(wind_prob <= 0.0 || wind_prob >= 1.0) {
        throw std::runtime_error("BlueBlackGame::Config::"\
                        "check wind_prob is invalid");
    }
}

bool BlueBlackGame::Config::check_point(const Point& p)
{
    return (p.x >= 0 && p.x < width &&
            p.y >= 0 && p.y < height);
}


const char *BlueBlackGame::ACT_STRINGS[] = {
    "UP", "DOWN", "RIGHT", "LEFT"
};

bool BlueBlackGame::valid_direct_str(const char *str)
{
    bool valid = false;

    for(int i = 0; i != ACT_COUNT; ++i) {
        if(strcmp(ACT_STRINGS[i], str) == 0) {
            valid = true;
            break;
        }
    }

    return valid;
}

Action BlueBlackGame::str_to_act(const char *direct)
{
    Action a = INVALID;

    for(int i = 0; i < ACT_COUNT; ++i) {
        if(strcmp(ACT_STRINGS[i], direct) {
            a = static_cast<Action>(i);
            break;
        }
    }

    return a;
}

BlueBlackGame::BlueBlackGame(const BlackBlueGame::Config& cfg)
{
    reset();
}

void BlueBlackGame::reset()
{
    gen_.seed(rd_());
    cur_pos_ = start_pos_;
    playable_ = true;
}

bool BlueBlackGame::action(int act, double& reward, int& next_state)
{
    if(!valid_act(act)) {
        throw std::runtime_error("Invalid act");
    }

    if(!playable_) {
       return false;
    }

    move_cur_pos(act);

    if(dis_(gen_) < wind_prob_) {
        move_cur_pos(wind_direct_);
    }

    if(cur_pos_ == blue_pos_) {
        reward = 1.0;
        playable_ = false;
    } else if(cur_pos_ == hole_pos_) {
        reward = -1.0;
        playable_ = false;
    } else {
        reward = 0.0;
        next_state = to_pos_idx(cur_pos_);
    }

    return playable_;
}

void BlueBlackGame::move_cur_pos(int act)
{
    switch(act) {
    case UP:
        if(cur_pos_.y > 0) {
            --(cur_pos_.y);
        }
        break;
    case DOWN:
        if(cur_pos_.y < BOARD_HEIGHT - 1) {
            ++(cur_pos_.y);
        }
        break;
    case LEFT:
        if(cur_pos_.x > 0) {
            --(cur_pos_.x);
        }
        break;
    case RIGHT:
        if(cur_pos_.y < BOARD_WIDTH - 1) {
            ++(cur_pos_.x);
        }
        break;
    default:
        std::cerr << "Invalid act(" << act << ")\n";
    }
}
