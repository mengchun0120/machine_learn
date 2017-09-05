#include <cstring>
#include <stdexcept>
#include <iostream>
#include <blue_black_game.hpp>
#include <conf_parser.hpp>

BlueBlackGame::Config::Config(const char *conf_file)
{
    init(conf_file);
}

void BlueBlackGame::Config::init(const char *conf_file)
{
   std::vector<ParamConfig> cfgs{
        ParamConfig("board_width", ParamConfig::INT_PARAM,
            true,reinterpret_cast<void *>(&board_width),
            lbound_check<int,false>(0)),
        ParamConfig("board_height", ParamConfig::INT_PARAM,
            true, reinterpret_cast<void *>(&board_height),
            lbound_check<int,false>(0)),
        ParamConfig("blue_pos_x", ParamConfig::INT_PARAM,
            true, reinterpret_cast<void *>(&blue_pos.x),
            lbound_check<int,false>(0)),
        ParamConfig("blue_pos_y", ParamConfig::INT_PARAM,
            true, reinterpret_cast<void *>(&blue_pos.y),
            lbound_check<int,false>(0)),
        ParamConfig("hole_pos_x", ParamConfig::INT_PARAM,
            true, reinterpret_cast<void *>(&hole_pos.x),
            lbound_check<int,false>(0)),
        ParamConfig("hole_pos_y", ParamConfig::INT_PARAM,
            true, reinterpret_cast<void *>(&hole_pos.y),
            lbound_check<int,false>(0)),
        ParamConfig("start_pos_x", ParamConfig::INT_PARAM,
            true, reinterpret_cast<void *>(&board_height),
            lbound_check<int,false>(0)),
        ParamConfig("start_pos_y", ParamConfig::INT_PARAM,
            true, reinterpret_cast<void *>(&board_height),
            lbound_check<int,false>(0)),
        ParamConfig("wind_prob", ParamConfig::DOUBLE_PARAM,
            true, reinterpret_cast<void *>(&board_height),
            lbound_check<double,false>(0.0) &&
            ubound_check<double,false>(1.0)),
        ParamConfig("wind_direct", ParamConfig::STRING_PARAM,
            true, reinterpret_cast<void *>(&board_height))
    };

    ConfParser parser(&cfgs);
    parser.read_config(conf_file);
}

bool BlueBlackGame::valid_direct(const void *direct)
{
    static std::vector<const char *> valid_dir{
        "UP", "DOWN", "RIGHT", "LEFT"
    };
    bool valid = false;

    for(auto it = valid_dir.begin(); it != valid_dir.end();
        ++it) {

        if(strcmp(*it,
            reinterpret_cast<const char *>(direct)) == 0) {

            valid = true;
            break;
        }
    }

    return valid;
}

BlueBlackGame::BlueBlackGame(int blue_pos_idx, int hole_pos_idx,
                             int start_pos_idx, double wind_prob,
                             int wind_direct):
    Game(BOARD_SIZE, 4),
    rd_(),
    dis_(0.0, 1.0)
{
    if(!valid_pos_idx(blue_pos_idx) ||
       !valid_pos_idx(hole_pos_idx) ||
       !valid_pos_idx(start_pos_idx)) {
        throw std::runtime_error("Invalid blue_pos_idx or hole_pos_idx"
                                 " or start_pos_idx");
    }

    if(blue_pos_idx == hole_pos_idx ||
       blue_pos_idx == start_pos_idx ||
       hole_pos_idx == start_pos_idx) {
        throw std::runtime_error("Overlapping blue_pos_idx, hole_pos_idx"
                                 " or start_pos_idx");
    }

    blue_pos_ = to_point(blue_pos_idx);
    hole_pos_ = to_point(hole_pos_idx);
    start_pos_ = to_point(start_pos_idx);

    if(wind_prob <= 0.0 && wind_prob > 1.0) {
        throw std::runtime_error("Invalid wind_prob");
    }

    wind_prob_ = wind_prob;

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
