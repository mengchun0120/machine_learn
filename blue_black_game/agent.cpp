#include "agent.hpp"
#include "game.hpp"

Agent::Agent(Game *game):
    rd_(),
    greedy_dis_(0.0, 1.0),
    act_dis_(0, game->num_actions()-1),
    game_(game)
{
    q_values_ = new double *[game->num_states()];
    for(int i = 0; i < game->num_states(); ++i) {
        q_values_[i] = new double[game->num_actions()];
    }
}

Agent::~Agent()
{
    for(int i = 0; i < game_->num_num_states(); ++i) {
        delete[] q_values_[i];
    }
    delete[] q_values_;
}

void Agent::learn(int num_episodes, double learn_rate, double greedy_prob)
{
    for(int episode = 0; episode < num_episodes; ++episode) {
        game->reset();
        while(true) {
            
        }
    }
}

void Agent::save_q(const char *file)
{
}

void Agent::read_q(const char *file)
{
}

void Agent::play_one_episode()
{
}

int Agent::select_action(double greedy_prob)
{
    int greedy_act;
    if(greedy_dis_(gen_) <= greedy_prob) {
        greedy_act = act_dis_(gen_);
    } else {
        double *q = q_values_[game_->cur_state()];
        double max_q = q_[0];
        greedy_act = 0;
        for(int i = 1; i < game_->num_actions(); ++i) {
            if(max_q < q[i]) {
                max_q = q[i];
                greedy_act = i;
            }
        }
    }
    return greedy_act;
}

