/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

class State;

class StateMachineCommand
{
    public:
    enum class Type
    {
        Nothing,
        Back,
        Change
    };

    StateMachineCommand(Type type = Type::Nothing)
    : type_(type)
    {

    }

    Type type() const
    {
        return type_;
    }

    protected:
    Type type_;
};

class StateMachineCommandBack : public StateMachineCommand
{
    public:
      StateMachineCommandBack()
      : StateMachineCommand(Type::Back)
      {

      }
};

class StateMachineCommandChange : public StateMachineCommand
{
    public:
      StateMachineCommandChange()
      : StateMachineCommand(Type::Change)
      {

      }

      void to(State * to)
      {
        to_ = to;
      }

      State * state() const
      {
        return to_;
      }

      State * to_;
};