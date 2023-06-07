/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0

  This implementation is heavily inspired by https://github.com/daschr/pico-ssd1306.
*/

#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "cilo72/hw/i2c_bus.h"
#include "cilo72/fonts/font_8x5.h"

namespace cilo72
{
  namespace ic
  {
    /**
     * @brief Class for controlling an SSD1306 OLED display via I2C.
     * \see  <a href="https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf">Datasheet</a>
     */
    class SSD1306
    {
    public:

      enum class Color
      {
        White,
        Black
      };

      /**
       * @brief Construct a new SSD1306 object.
       *
       * @param i2cBus The I2C bus to use for communication with the display.
       * @param useSA0 If true, the display's SA0 pin is tied to GND.
       */
      SSD1306(const cilo72::hw::I2CBus &i2cBus, bool useSA0 = true);

      /**
       * @brief Clear the display's buffer.
       *
       */
      void clear(Color color = Color::Black);

      /**
       * @brief Update the display with the contents of the buffer.
       *
       */
      void update();

      /**
       * @brief Draw a line on the display.
       *
       * @param x1 The starting X coordinate.
       * @param y1 The starting Y coordinate.
       * @param x2 The ending X coordinate.
       * @param y2 The ending Y coordinate.
       */
      void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color color = Color::White);

      /**
       * @brief Draw a pixel on the display.
       *
       * @param x The X coordinate.
       * @param y The Y coordinate.
       */
      void drawPixel(uint32_t x, uint32_t y, Color color = Color::White);

      /**
       * @brief Draw a filled square on the display.
       *
       * @param x The X coordinate of the top-left corner of the square.
       * @param y The Y coordinate of the top-left corner of the square.
       * @param width The width of the square.
       * @param height The height of the square.
       */
      void drawSquare(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color = Color::White);

      /**
       * @brief Draw an empty square on the display.
       *
       * @param x The X coordinate of the top-left corner of the square.
       * @param y The Y coordinate of the top-left corner of the square.
       * @param width The width of the square.
       * @param height The height of the square.
       */
      void drawEmptySquare(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color = Color::White);

      /**
       * @brief Draw a character on the display.
       *
       * @param x The X coordinate.
       * @param y The Y coordinate.
       * @param scale The character scaling factor.
       * @param c The character to draw.
       * @param font The font to use. Default is cilo72::fonts::Font8x5().
       */
      void drawChar(uint32_t x, uint32_t y, uint32_t scale, char c, Color color = Color::White, const cilo72::fonts::Font &font = cilo72::fonts::Font8x5());

      /**
       * @brief Draw a string on the display.
       *
       * @param x The X coordinate.
       * @param y The Y coordinate.
       * @param scale The character scaling factor.
       * @param s The string to draw.
       * @param font The font to use. Default is cilo72::fonts::Font8x5().
       */
      void drawString(uint32_t x, uint32_t y, uint32_t scale, const char *s, Color color = Color::White, const cilo72::fonts::Font &font = cilo72::fonts::Font8x5());

      /**
       * @brief Turns off the display.
       */
      void powerOff();

      /**
       * @brief Turns on the display.
       */
      void powerOn();

      /**
       * @brief Sets the contrast of the display.
       * @param val The contrast value (0-255).
       */
      void contrast(uint8_t val);

      /**
       * @brief Inverts the display.
       * @param inv Whether to invert the display (true) or not (false).
       */
      void invert(uint8_t inv);

      uint16_t height() const
      {
        return height_;
      }

      uint16_t width() const
      {
        return width_;
      }
    private:
      const cilo72::hw::I2CBus &i2cBus_;
      uint16_t height_;
      uint16_t width_;
      bool external_vcc_;
      uint8_t address_;
      uint8_t pages_;
      uint8_t *buffer_;
      size_t bufsize_;

      /**
       * @brief Initializes the display.
       * @return True if initialization was successful, false otherwise.
       */
      bool init();

      /**
       * @brief Writes a value to the display.
       * @param val The value to write.
       * @return True if the write was successful, false otherwise.
       */
      bool write(uint8_t val);
    };
  }
}
