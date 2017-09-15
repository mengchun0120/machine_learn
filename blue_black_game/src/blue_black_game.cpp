#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <cassert>
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
                    reinterpret_cast<void *>(&height),
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
                    assign_str)
    };

    int count = static_cast<int>(sizeof(cfgs) /
                                 sizeof(ParamConfig));

    ConfParser parser(cfgs, count);
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

    if(blue_pos == hole_pos || blue_pos == start_pos ||
       hole_pos == start_pos) {
        throw std::runtime_error("BlueBlackGame::Config::"\
                        "check overlapping blue_pos, "\
                        "start_pos, hole_pos");
    }

    if(BlueBlackGame::str_to_act(wind_direct) ==
       BlueBlackGame::ACT_INVALID) {
        std::cout << wind_direct << std::endl;
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

BlueBlackGame::Action
BlueBlackGame::str_to_act(const char *act_str)
{
    Action a = ACT_INVALID;

    for(int i = 0; i < ACT_COUNT; ++i) {
        if(strcmp(ACT_STRINGS[i], act_str) == 0) {
            a = static_cast<Action>(i);
            break;
        }
    }

    return a;
}

BlueBlackGame::BlueBlackGame(const BlueBlackGame::Config& cfg):
    Game(cfg.width * cfg.height, ACT_COUNT),
    width_(cfg.width),
    height_(cfg.height),
    num_states_(width_ * height_),
    blue_pos_(cfg.blue_pos),
    hole_pos_(cfg.hole_pos),
    start_pos_(cfg.start_pos),
    wind_prob_(cfg.wind_prob),
    wind_direct_(str_to_act(cfg.wind_direct)),
    dis_(0.0, 1.0)
{
    reset();
}

void BlueBlackGame::reset()
{
    gen_.seed(rd_());
    cur_pos_ = start_pos_;
    playable_ = true;
}

bool BlueBlackGame::action(int act, double& reward,
                           int& next_state, bool debug)
{
    assert(valid_act(act));

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
        next_state = get_state(cur_pos_);
    }

    if(debug) {
        std::cout << "BlueBlackGame::action act=" << act
                  << " reward=" << reward << " cur_pos="
                  << cur_pos_ << std::endl;
    }

    return playable_;
}

bool BlueBlackGame::valid_act(int state, int act) const
{
    assert(valid_act(act) && valid_state(state));

    bool valid = true;
    Point p;

    get_position(state, p);

    switch(act) {
    case ACT_UP:
        valid = (p.y > 0);
        break;
    case ACT_DOWN:
        valid = (p.y < height_ - 1);
        break;
    case ACT_LEFT:
        valid = (p.x > 0);
        break;
    case ACT_RIGHT:
        valid = (p.x < width_ - 1);
        break;
    }

    return valid;
}

void BlueBlackGame::get_position(int state, Point& p) const
{
    assert(valid_state(state));

    p.y = state / width_;
    p.x = state % width_;
}

void BlueBlackGame::move_cur_pos(int act)
{
    switch(act) {
    case ACT_UP:
        if(cur_pos_.y > 0) {
            --(cur_pos_.y);
        }
        break;
    case ACT_DOWN:
        if(cur_pos_.y < height_ - 1) {
            ++(cur_pos_.y);
        }
        break;
    case ACT_LEFT:
        if(cur_pos_.x > 0) {
            --(cur_pos_.x);
        }
        break;
    case ACT_RIGHT:
        if(cur_pos_.x < width_ - 1) {
            ++(cur_pos_.x);
        }
        break;
    default:
        std::cerr << "Invalid act(" << act << ")\n";
    }
}
