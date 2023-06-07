/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include "menuitem.h"
#include "cilo72/ic/ssd1306.h"
#include "cilo72/fonts/font_8x5.h"
#include <list>
class Menu
{
public:
    Menu(cilo72::ic::SSD1306 &oled, const cilo72::fonts::Font &font = cilo72::fonts::Font8x5());
    void add(MenuItem *item);
    void reset();
    void up();
    void down();
    const MenuItem *selected() const;
    void draw();

private:
    cilo72::ic::SSD1306 &oled_;
    const cilo72::fonts::Font &font_;
    std::list<MenuItem *> items_;
    uint32_t index_;
    void updateSelect();
};