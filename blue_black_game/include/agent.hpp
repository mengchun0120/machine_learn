#ifndef __AGENT_HPP__
#define __AGENT_HPP__

#include <random>
#include <functional>

class Game;

class Agent {
public:
    typedef std::function<void(int,int,double,int)> GameFunc;

    Agent(Game *game, double q_init_max);

    virtual ~Agent();

    void learn(int num_episodes, double lambda, double learn_rate,
               double greedy_prob, int debug_steps, bool random_init=true);

    void save_q(const char *file);

    void read_q(const char *file);

    void play_one_episode(GameFunc f=nullptr);

    int select_action(int cur_state, double greedy_prob, bool debug=false);

    double update(int cur_state, int action, double reward, int next_state,
                  double lambda, double learn_rate, bool debug=false);

protected:
    void alloc_q();

    void release_q();

    void init_q();

    int select_action_greedily(int cur_state);

    int select_action_randomly(int cur_state);

    Game *game_;
    double **q_values_;
    int num_states_, num_actions_;
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_real_distribution<> greedy_act_dis_;
    std::uniform_int_distribution<> random_act_dis_;
    std::uniform_real_distribution<> q_dis_;
};

#endif
