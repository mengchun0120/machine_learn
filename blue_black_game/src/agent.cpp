#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <math.h>
#include <agent.hpp>
#include <game.hpp>
#include <conf_parser.hpp>

Agent::Config::Config(const char *conf_file)
{
    init(conf_file);
}

void Agent::Config::init(const char *conf_file)
{
    q_init_max = 0.1;
    debug_steps = 50;

    std::vector<ParamConfig> cfgs{
        ParamConfig("num_episodes", ParamConfig::INT_PARAM,
              true, reinterpret_cast<void *>(&num_episodes),
              lbound_check<int,false>(0)),
        ParamConfig("q_init_max", ParamConfig::DOUBLE_PARAM,
              false, reinterpret_cast<void *>(&q_init_max),
              lbound_check<double,false>(0.0)),
        ParamConfig("lambda", ParamConfig::DOUBLE_PARAM,
              true, reinterpret_cast<void *>(&lambda),
              lbound_check<double,false>(0.0)),
        ParamConfig("learn_rate", ParamConfig::DOUBLE_PARAM,
              true, reinterpret_cast<void *>(&learn_rate),
              lbound_check<double,false>(0.0)),
        ParamConfig("greedy_prob", ParamConfig::DOUBLE_PARAM,
              true, reinterpret_cast<void *>(&greedy_prob),
              lbound_check<double,false>(0.0) &&
              ubound_check<double,false>(1.0)),
        ParamConfig("debug_steps", ParamConfig::INT_PARAM,
              false, reinterpret_cast<void *>(&debug_steps),
              lbound_check<int,false>(0))
    };

    ConfParser parser(&cfgs);
    parser.read_config(conf_file);
}

Agent::Agent(Game *game):
    game_(game),
    num_states_(game->num_states()),
    num_actions_(game->num_actions()),
    rd_(),
    greedy_act_dis_(0.0, 1.0),
    random_act_dis_(0, num_actions_-1)
{
    alloc_q();
}

Agent::~Agent()
{
    release_q();
}

void Agent::alloc_q()
{
    q_values_ = new double *[num_states_];
    for(int i = 0; i < num_states_; ++i) {
        q_values_[i] = new double[num_actions_];
    }
}

void Agent::release_q()
{
    for(int i = 0; i < num_states_; ++i) {
        delete[] q_values_[i];
    }
    delete[] q_values_;
}

void Agent::init_q(double q_max)
{
    std::mt19937 g(rd_());
    std::uniform_real_distribution<> dis(-q_max, q_max);

    for(int i = 0; i < num_states_; ++i) {
        double *q = q_values_[i];
        for(int j = 0; j < num_actions_; ++j, ++q) {
            *q = dis(g);
        }
    }
}

void Agent::learn(const Agent::Config *cfg)
{
    int act, next_state, cur_state;
    double reward;
    bool playable;

    if(cfg->random_init) init_q(cfg->q_init_max);

    double abs_delta;

    for(int episode = 0; episode < cfg->num_episodes;
        ++episode) {

        int num_steps = 0;
        bool debug;

        game_->reset();
        cur_state = game_->cur_state();
        do {
            ++num_steps;

            if(cfg->debug_steps > 0) {
                debug = (num_steps % cfg->debug_steps) == 0;
            } else {
                debug = false;
            }

            act = select_action(cur_state, cfg->greedy_prob,
                                debug);

            playable = game_->action(act, reward, next_state);

            abs_delta = update(cur_state, act, reward,
                               next_state, cfg->lambda,
                               cfg->learn_rate,
                               debug || !playable);
            cur_state = next_state;
        } while(playable && abs_delta > 1.0e-6);
    }
}

double Agent::update(int cur_state, int action, double reward,
                     int next_state, double lambda,
                     double learn_rate, bool debug)
{
    double *q = q_values_[next_state];
    double max_q = q[0];

    for(int i = 1; i < num_actions_; ++i, ++q) {
        if(*q > max_q) max_q = *q;
    }

    double delta = learn_rate * (reward + lambda * max_q);
    q_values_[cur_state][action] += delta;

    if(debug) {
        std::cout << "update q_values for state=" << cur_state
                  << " act=" << action << " delta=" << delta
                  << " cur_q=" << q_values_[cur_state][action]
                  << std::endl;
    }

    return fabs(delta);
}

void Agent::save_q(const char *file)
{
    std::ofstream os(file,
                     std::ios::out | std::ios::binary);

    os.exceptions(std::ofstream::failbit |
                  std::ofstream::badbit);

    os.write(reinterpret_cast<char *>(&num_states_),
             sizeof(num_states_));
    os.write(reinterpret_cast<char *>(&num_actions_),
             sizeof(num_actions_));

    for(int i = 0; i < num_states_; ++i) {
        os.write(reinterpret_cast<char *>(q_values_[i]),
                 sizeof(double) * num_actions_);
    }
}

void Agent::read_q(const char *file)
{
    std::ifstream is(file, std::ios::in | std::ios::binary);

    release_q();
    alloc_q();

    is.exceptions(std::ifstream::failbit |
                  std::ofstream::badbit);

    is.read(reinterpret_cast<char *>(&num_states_),
            sizeof(num_states_));
    is.read(reinterpret_cast<char *>(&num_actions_),
            sizeof(num_actions_));

    for(int i = 0; i < num_states_; ++i) {
        is.read(reinterpret_cast<char *>(q_values_[i]),
                sizeof(double) * num_actions_);
    }
}

void Agent::play_one_episode(GameFunc f)
{
    bool playable;
    int cur_state, act, next_state;
    double reward;

    game_->reset();
    cur_state = game_->cur_state();
    do {
        act = select_action_greedily(cur_state);
        playable = game_->action(act, reward, next_state);
        if(f) f(cur_state, act, reward, next_state);
        cur_state = next_state;
    } while(playable);
}

int Agent::select_action(int cur_state, double greedy_prob,
                         bool debug)
{
    int act;
    if(greedy_act_dis_(gen_) <= greedy_prob) {
        act = select_action_greedily(cur_state);
        if(debug) {
            std::cout << "select greedy act=" << act
                      << std::endl;
        }
    } else {
        act = select_action_randomly(cur_state);
        if(debug) {
            std::cout << "select random act=" << act
                      << std::endl;
        }
    }
    return act;
}

int Agent::select_action_greedily(int cur_state)
{
    int act = 0;
    double *q = q_values_[cur_state];
    double max_q = q[0];

    for(int i = 1; i < num_actions_; ++i) {
        if(max_q < q[i]) {
            max_q = q[i];
            act = i;
        }
    }
    return act;
}

int Agent::select_action_randomly(int cur_state)
{
    return random_act_dis_(gen_);
}
