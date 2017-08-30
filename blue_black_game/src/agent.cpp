#include <iostream>
#include <fstream>
#include <stdexcept>
#include <math.h>
#include <agent.hpp>
#include <game.hpp>

Agent::Agent(Game *game, double q_init_max):
    game_(game),
    num_states_(game->num_states()),
    num_actions_(game->num_actions()),
    rd_(),
    greedy_act_dis_(0.0, 1.0),
    random_act_dis_(0, num_actions_-1),
    q_dis_(-q_init_max, q_init_max)
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

void Agent::init_q()
{
    for(int i = 0; i < num_states_; ++i) {
        double *q = q_values_[i];
        for(int j = 0; j < num_actions_; ++j, ++q) {
            *q = q_dis_(gen_);
        }
    }
}


void Agent::learn(int num_episodes, double lambda, double learn_rate,
                  double greedy_prob, int debug_steps, bool random_init)
{
    int act, next_state, cur_state;
    double reward;
    bool playable;

    if(greedy_prob <= 0.0 || greedy_prob >= 1.0) {
        throw std::runtime_error("Agent::learn: Invalid greedy_prob");
    }

    if(learn_rate <= 0.0) {
        throw std::runtime_error("Agent::learn: Invalid learn_rate");
    }

    if(random_init) init_q();

    double abs_delta;

    for(int episode = 0; episode < num_episodes; ++episode) {
        std::cout << "ep " << episode << std::endl;

        int num_steps = 0;
        bool debug;

        game_->reset();
        cur_state = game_->cur_state();
        do {
            ++num_steps;

            if(debug_steps > 0) {
                debug = (num_steps % debug_steps) == 0;
            } else {
                debug = false;
            }

            act = select_action(cur_state, greedy_prob, debug);
            playable = game_->action(act, reward, next_state);
            abs_delta = update(cur_state, act, reward, next_state, lambda,
                               learn_rate, debug || !playable);
            cur_state = next_state;
        } while(playable && abs_delta > 1.0e-6);
    }
}

double Agent::update(int cur_state, int action, double reward, int next_state,
                   double lambda, double learn_rate, bool debug)
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
                  << " cur_q=" << q_values_[cur_state][action] << std::endl;
    }

    return fabs(delta);
}

void Agent::save_q(const char *file)
{
    std::ofstream os(file, std::ios::out | std::ios::binary);

    os.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    os.write(reinterpret_cast<char *>(&num_states_), sizeof(num_states_));
    os.write(reinterpret_cast<char *>(&num_actions_), sizeof(num_actions_));

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

    is.exceptions(std::ifstream::failbit | std::ofstream::badbit);

    is.read(reinterpret_cast<char *>(&num_states_), sizeof(num_states_));
    is.read(reinterpret_cast<char *>(&num_actions_), sizeof(num_actions_));

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

int Agent::select_action(int cur_state, double greedy_prob, bool debug)
{
    int act;
    if(greedy_act_dis_(gen_) <= greedy_prob) {
        act = select_action_greedily(cur_state);
        if(debug) {
            std::cout << "select greedy act=" << act << std::endl;
        }
    } else {
        act = select_action_randomly(cur_state);
        if(debug) {
            std::cout << "select random act=" << act << std::endl;
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
