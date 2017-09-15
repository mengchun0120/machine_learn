#!/bin/bash

set -x

../build/src/bbgame train -g "game_4x4.conf" \
                    -a "agent_4x4.conf" \
                    -w "bbgame_q_values_4x4.bin"
