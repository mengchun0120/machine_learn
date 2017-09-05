#include <conf_parser.hpp>
#include <agent_config.hpp>

AgentConfig::AgentConfig(const char *conf_file)
{
    init(conf_file);
}

void AgentConfig::init(const char *conf_file)
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


