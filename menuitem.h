/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include "state.h"

class MenuItem
{
  public:
    MenuItem(const char * text, State * next = nullptr);
    const char * text() const { return text_; }
    bool isSelected() const { return selected_; }
    void select(bool value)  { selected_ = value; }
    State * next() const { return next_; }
  private:
    const char * text_;
    bool selected_;
    State * next_;
};
