#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <conf_parser.hpp>
#include <blue_black_game.hpp>
#include <agent.hpp>
#include <record_game_func.hpp>

void usage()
{
    std::cerr << "Usage: \n"
              << "bbgame train -g game_conf -a agent_conf "
              << "[-p prev_q] -w cur_q\n"
              << "bbgame play -g game_conf -p q_file "
              << "[-r rec_file]\n";
}

void parse_param_train(int argc, char **argv, char *&game_conf,
               char *&agent_conf, char *&prev_q, char *&cur_q)
{
    game_conf = nullptr;
    agent_conf = nullptr;
    prev_q = nullptr;
    cur_q = nullptr;

    int i = 2;

    while(i < argc) {
        if(strcmp(argv[i], "-g") == 0 && (i+1) < argc) {
            game_conf = argv[i+1];
            i += 2;
        } else if(strcmp(argv[i], "-a") == 0 && (i+1) < argc) {
            agent_conf = argv[i+1];
            i += 2;
        } else if(strcmp(argv[i], "-p") == 0 && (i+1) < argc) {
            prev_q = argv[i+1];
            i += 2;
        } else if(strcmp(argv[i], "-w") == 0 && (i+1) < argc) {
            cur_q = argv[i+1];
            i += 2;
        } else {
            throw std::runtime_error("parse_param_train: " \
                                     "unknown option");
        }
    }

    if(!game_conf || !agent_conf || !cur_q) {
        throw std::runtime_error("parse_param_trai: " \
                                 "missing option");
    }
}

void parse_param_play(int argc, char **argv, char *&game_conf,
                      char *&q_file, char *&rec_file)
{
    game_conf = nullptr;
    q_file = nullptr;
    rec_file = nullptr;

    int i = 2;
    while(i < argc) {
        if(strcmp(argv[i], "-g") == 0 && (i+1) < argc) {
            game_conf = argv[i+1];
            i += 2;
        } else if(strcmp(argv[i], "-p") == 0 && (i+1) < argc) {
            q_file = argv[i+1];
            i += 2;
        } else if(strcmp(argv[i], "-r") == 0 && (i+1) < argc) {
            rec_file = argv[i+1];
            i += 2;
        } else {
            throw std::runtime_error("parse_param_play: "\
                                     "unknown option");
        }
    }

    if(!game_conf || !q_file) {
        throw std::runtime_error("parse_param_play: "\
                                 "missing option");
    }
}

void train(int argc, char **argv)
{
    char *game_conf, *agent_conf, *prev_q, *cur_q;

    parse_param_train(argc, argv, game_conf, agent_conf,
                      prev_q, cur_q);

    BlueBlackGame::Config game_cfg(game_conf);
    BlueBlackGame game(game_cfg);

    Agent agent(&game);
    Agent::Config agent_cfg(agent_conf);

    if(prev_q) {
        agent.read_q(prev_q);
    }

    agent.learn(&agent_cfg);

    agent.save_q(cur_q);
}

void play(int argc, char **argv)
{
    char *game_conf, *q_file, *rec_file;

    parse_param_play(argc, argv, game_conf, q_file, rec_file);

    BlueBlackGame::Config game_cfg(game_conf);
    BlueBlackGame game(game_cfg);

    Agent agent(&game);

    if(rec_file) {
        std::ofstream os(rec_file);
        RecordGameFunc rec_func(os);

        agent.play_one_episode(rec_func);

        os.close();

    } else {
        RecordGameFunc rec_func(std::cout);

        agent.play_one_episode(rec_func);
    }

}

int main(int argc, char **argv)
{
    if(argc < 3) {
        usage();
        exit(1);
    }

    if(strcmp(argv[1], "train") == 0) {
        train(argc, argv);
    } else if(strcmp(argv[1], "play") == 0) {
        play(argc, argv);
    }
}

