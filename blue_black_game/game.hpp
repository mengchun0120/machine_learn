#ifndef __GAME_HPP__
#define __GAME_HPP__

class Game {
public:
    virtual void reset() = 0;
    virtual bool action(int act, double& reward, int& next_state) = 0;
};

#endif
