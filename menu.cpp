/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "menu.h"

Menu::Menu(cilo72::ic::SSD1306 &oled, const cilo72::fonts::Font &font)
    : oled_(oled), font_(font), index_(0)
{
}

void Menu::add(MenuItem *item)
{
    items_.push_back(item);
}

void Menu::reset()
{
    index_ = 0;
    updateSelect();
}

void Menu::up()
{
    if (index_ > 0)
    {
        index_--;
    }

    updateSelect();
}

void Menu::down()
{
    if (index_ < (items_.size() - 1))
    {
        index_++;
    }
    updateSelect();
}

const MenuItem *Menu::selected() const
{
    uint32_t i = 0;
    for (auto item : items_)
    {
        if (i == index_)
        {
            return item;
        }
        i++;
    }
    return items_.front();
}

void Menu::draw()
{
    int x = 2;
    int y = 1;
    constexpr uint32_t scale = 2;
    oled_.clear();
    for (auto item : items_)
    {
        if (item->isSelected())
        {
            oled_.drawSquare(x, y, oled_.width(), font_.height() * scale, cilo72::ic::SSD1306::Color::White);
            oled_.drawString(x, y, scale, item->text(), cilo72::ic::SSD1306::Color::Black, font_);
        }
        else
        {
            oled_.drawString(x, y, scale, item->text(), cilo72::ic::SSD1306::Color::White, font_);
        }
        y += font_.height() * scale;
    }
    oled_.update();
}

void Menu::updateSelect()
{
    uint32_t i = 0;
    for (auto item : items_)
    {
        item->select(i == index_);
        i++;
    }
}
