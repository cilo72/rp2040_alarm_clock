/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include "state.h"
#include "statemachinecommand.h"

class StateMachine
{
    public:
    StateMachine(State * state);

    void run();
    private:
    State * state_;
};
