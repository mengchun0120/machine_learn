#ifndef __RECORD_GAME_FUNC_HPP__
#define __RECORD_GAME_FUNC_HPP__

#include <fstream>

class RecordGameFunc {
public:
    RecordGameFunc(std::ostream& os);

    void operator()(int cur_state, int act, double reward,
                    int next_state);

private:
    std::ostream& os_;
    int step_;
};

#endif

