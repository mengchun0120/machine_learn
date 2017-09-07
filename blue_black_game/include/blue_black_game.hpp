#ifndef __BLUE_BLACK_GAME_HPP__
#define __BLUE_BLACK_GAME_HPP__

#include <random>
#include <game.hpp>

class BlueBlackGame: public Game {
public:
    enum Action {
        ACT_UP,
        ACT_DOWN,
        ACT_RIGHT,
        ACT_LEFT,
        ACT_INVALID
    };

    static const std::vector<const char *> ACT_STRINGS;

    struct Config {
        static constexpr int MIN_WIDTH = 4;
        static constexpr int MIN_HEIGHT = 4;

        Config(const char *conf_file);

        void init(const char *conf_file);

        bool check();

        int width;
        int height;
        Point blue_pos;
        Point hole_pos;
        Point start_pos;
        double wind_prob;
        char wind_direct[10];
    };

    static bool valid_act(int act)
    {
        return act >= UP && act <= LEFT;
    }

    BlueBlackGame(const Config& cfg);

    void reset();

    bool action(int act, double& reward, int& next_state);

    int cur_state() const
    {
        return to_pos_idx(cur_pos_);
    }

protected:
    Point blue_pos_;
    Point hole_pos_;
    Point start_pos_;
    Point cur_pos_;
    double wind_prob_;
    int wind_direct_;
    bool playable_;
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_real_distribution<> dis_;

    void move_cur_pos(int act);
};

#endif
