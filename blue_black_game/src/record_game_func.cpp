#include <record_game_func.hpp>

RecordGameFunc::RecordGameFunc(std::ostream& os):
    os_(os),
    step_(0)
{
}

void RecordGameFunc::operator()(int cur_state, int act,
                                double reward, int next_state)
{
    os_ << step_ << ' ' << cur_state << ' ' << act << ' '
        << reward << ' ' << next_state << std::endl;

    ++step_;
}
