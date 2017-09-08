#ifndef __BLUE_BLACK_GAME_HPP__
#define __BLUE_BLACK_GAME_HPP__

#include <random>
#include <game.hpp>
#include <point.hpp>

class BlueBlackGame: public Game {
public:
    enum Action {
        ACT_UP,
        ACT_DOWN,
        ACT_RIGHT,
        ACT_LEFT,
        ACT_INVALID
    };

    static constexpr int ACT_COUNT = 4;
    static const char *ACT_STRINGS[ACT_COUNT];

    struct Config {
        static constexpr int MIN_WIDTH = 4;
        static constexpr int MIN_HEIGHT = 4;

        Config(const char *conf_file);

        void init(const char *conf_file);

        void check();

        bool check_point(const Point& p);

        int width;
        int height;
        Point blue_pos;
        Point hole_pos;
        Point start_pos;
        double wind_prob;
        char wind_direct[10];
    };

    static Action str_to_act(const char *act_str);

    static bool valid_act(int act)
    {
        return act >= 0 && act < ACT_COUNT;
    }

    BlueBlackGame(const Config& cfg);

    void reset();

    bool action(int act, double& reward, int& next_state);

    int cur_state() const
    {
        return get_state(cur_pos_);
    }

    int get_state(const Point& p) const
    {
        return p.y * width_ + p.x;
    }

protected:
    int width_;
    int height_;
    Point blue_pos_;
    Point hole_pos_;
    Point start_pos_;
    Point cur_pos_;
    double wind_prob_;
    Action wind_direct_;
    bool playable_;
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_real_distribution<> dis_;

    void move_cur_pos(int act);
};

#endif
