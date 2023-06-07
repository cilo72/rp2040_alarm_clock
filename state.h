/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <functional>
#include "statemachinecommand.h"

class State
{
    public:
    State()
    : onEnter_([](){})
    , onRun_([&](State & state) -> const StateMachineCommand * { return nothing(); })
    , onExit_([](){})
    {

    }

    void back()
    {

    }

    const StateMachineCommand * run() 
    {
        return onRun_(*this);
    }

    void setOnRun(std::function<const StateMachineCommand *(State & state)> f)
    {
        onRun_ = f;
    }

    void setOnEnter(std::function<void()> f)
    {
        onEnter_ = f;
    }

    void onEnter()
    {
        onEnter_();
    }

    void setOnExit(std::function<void()> f)
    {
        onExit_ = f;
    }

    void onExit()
    {
        onExit_();
    }

    const StateMachineCommand * nothing() const
    {
        return &nothing_;
    }

    const StateMachineCommand *  changeTo(State * next)
    {
        change_.to(next);
        return &change_;
    }

    StateMachineCommand nothing_;
    StateMachineCommandChange change_;
    std::function<void()> onEnter_;
    std::function<const StateMachineCommand *(State & state)> onRun_;
    std::function<void()> onExit_;
};