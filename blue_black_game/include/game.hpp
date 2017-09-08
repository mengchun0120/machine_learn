#ifndef __GAME_HPP__
#define __GAME_HPP__

class Game {
public:
    Game(int num_states, int num_actions):
        num_states_(num_states),
        num_actions_(num_actions)
    {}

    virtual void reset() = 0;

    virtual bool action(int act, double& reward,
                        int& next_state) = 0;

    int num_states() const
    {
        return num_states_;
    }

    int num_actions() const
    {
        return num_actions_;
    }

    virtual int cur_state() const = 0;

protected:
    int num_states_;
    int num_actions_;
};

#endif
