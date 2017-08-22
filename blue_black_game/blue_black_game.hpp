#ifndef __BLUE_BLACK_GAME_HPP__
#define __BLUE_BLACK_GAME_HPP__

#include <random>
#include "game.hpp"

struct Point {
    Point(int x=0, int y=0): this->x(x), this->y(y) {}

    Point(const Point& p): x(p.x), y(p.y) {}

    Point& operator=(const Point& p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    bool operator==(const Point& p)
    {
        return x == p.x && y == p.y;
    }

    int x;
    int y;
};

class BlueBlackGame: public Game {
public:
    enum Action {
        UP,
        DOWN,
        RIGHT,
        LEFT
    };

    constexpr static int BOARD_WIDTH = 4;
    constexpr static int BOARD_HEIGHT = 4;
    constexpr static int BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;

    static bool valid_pos_idx(int pos_idx)
    {
        return pos >= 0 && pos < BOARD_SIZE;
    }

    static Point to_point(int pos_idx)
    {
        Point p;
        p.x = pos_idx % BOARD_WIDTH;
        p.y = pos_idx / BOARD_WIDTH;
        return p;
    }

    static int to_pos_idx(const Point& p)
    {
        return p.y * BOARD_WIDTH + p.x;
    }

    static bool valid_act(int act)
    {
        return act >= UP && act <= LEFT;
    }

    BlueBlackGame(int blue_pos_idx, int hole_pos_idx, int start_pos_idx, 
                  double wind_prob, int wind_direct);

    void reset();

    bool action(int act, double& reward, int& next_state); 

protected:
    Point blue_pos_;
    Point hold_pos_;
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
 
