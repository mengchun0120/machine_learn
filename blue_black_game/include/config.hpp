#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

struct GameConfig {
    GameConfig():
        blue_pos_idx(-1),
        hole_pos_idx(-1),
        start_pos_idx(-1),
        wind_prob(-1.0),
        wind_direct(-1)
    {}

    int blue_pos_idx;
    int hole_pos_idx;
    int start_pos_idx;
    double wind_prob;
    int wind_direct;
};

struct AgentConfig {
    AgentConfig():
        num_episodes(-1),
        lambda(-1.0),
        learn_rate(-1.0),
        greedy_prob(-1.0),
        debug_steps(-1)
    {}

    char *conf_file;
    char *q_file;
    int num_episodes;
    double lambda;
    double learn_rate;
    double greedy_prob;
    int debug_steps;
};

#endif

