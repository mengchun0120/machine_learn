#ifndef __AGENT_CONFIG_HPP__
#define __AGENT_CONFIG_HPP__

struct AgentConfig {
    AgentConfig(const char *conf_file);

    void init(const char *conf_file);

    int num_episodes;
    double q_init_max;
    double lambda;
    double learn_rate;
    double greedy_prob;
    int debug_steps;
    bool random_init;
};

#endif
