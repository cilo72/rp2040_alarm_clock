/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "menuitem.h"

MenuItem::MenuItem(const char *text, State *next)
    : text_(text), selected_(false), next_(next)
{
}
