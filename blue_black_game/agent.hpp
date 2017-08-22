#ifndef __AGENT_HPP__
#define __AGENT_HPP__

#include <random>

class Game;

class Agent {
public:
    Agent(Game *game);

    virtual ~Agent();

    void learn(int num_episodes, double learn_rate, double greedy_prob);

    void save_q(const char *file);

    void read_q(const char *file);

    void play_one_episode();

    int select_action(double greedy_prob);

protected:
    Game *game_;
    double **q_values_;
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_real_distribution<> greedy_dis_;
    std::uniform_int_distribution<> act_dis_;
};

#endif
