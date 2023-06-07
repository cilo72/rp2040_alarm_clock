/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/ic/ssd1306.h"
#include "cilo72/fonts/font.h"
#include <string.h>



namespace cilo72
{
    namespace
    {
        typedef enum
        {
            SET_CONTRAST = 0x81,
            SET_ENTIRE_ON = 0xA4,
            SET_NORM_INV = 0xA6,
            SET_DISP = 0xAE,
            SET_MEM_ADDR = 0x20,
            SET_COL_ADDR = 0x21,
            SET_PAGE_ADDR = 0x22,
            SET_DISP_START_LINE = 0x40,
            SET_SEG_REMAP = 0xA0,
            SET_MUX_RATIO = 0xA8,
            SET_COM_OUT_DIR = 0xC0,
            SET_DISP_OFFSET = 0xD3,
            SET_COM_PIN_CFG = 0xDA,
            SET_DISP_CLK_DIV = 0xD5,
            SET_PRECHARGE = 0xD9,
            SET_VCOM_DESEL = 0xDB,
            SET_CHARGE_PUMP = 0x8D
        } ssd1306_command_t;

        constexpr uint8_t address0 = 0x3C;
        constexpr uint8_t address1 = 0x3D;

        inline static void swap(int32_t *a, int32_t *b)
        {
            int32_t *t = a;
            *a = *b;
            *b = *t;
        }
    }

    namespace ic
    {
        SSD1306::SSD1306(const cilo72::hw::I2CBus &i2cBus, bool useSA0)
            : i2cBus_(i2cBus)
            , height_(64)
            , width_(128)
            , external_vcc_(false)
            , address_(useSA0 ? address0 : address1)
            , pages_(height_ / 8)
            , bufsize_(pages_ * width_)
        {
            buffer_ = new uint8_t[bufsize_ + 1];
            assert(buffer_);
            ++buffer_;
            init();
            clear();
            update();
        }

        bool SSD1306::init()
        {
            uint8_t cmds[] = {
                SET_DISP,
                // timing and driving scheme
                SET_DISP_CLK_DIV,
                0x80,
                SET_MUX_RATIO,
                uint8_t(height_ - 1),
                SET_DISP_OFFSET,
                0x00,
                // resolution and layout
                SET_DISP_START_LINE,
                // charge pump
                SET_CHARGE_PUMP,
                (uint8_t)(external_vcc_ ? 0x10 : 0x14),
                SET_SEG_REMAP | 0x01,   // column addr 127 mapped to SEG0
                SET_COM_OUT_DIR | 0x08, // scan from COM[N] to COM0
                SET_COM_PIN_CFG,
                (uint8_t)(width_ > 2 * height_ ? 0x02 : 0x12),
                // display
                SET_CONTRAST,
                0xff,
                SET_PRECHARGE,
                (uint8_t)(external_vcc_ ? 0x22 : 0xF1),
                SET_VCOM_DESEL,
                0x30,          // or 0x40?
                SET_ENTIRE_ON, // output follows RAM contents
                SET_NORM_INV,  // not inverted
                SET_DISP | 0x01,
                // address setting
                SET_MEM_ADDR,
                0x00, // horizontal
            };

            for (size_t i = 0; i < sizeof(cmds); ++i)
            {
                write(cmds[i]);
            }
            return true;
        }

        void SSD1306::clear(Color color)
        {
            switch(color)
            {
                case Color::White:
                  memset(buffer_, 0xFF, bufsize_);
                break;

                case Color::Black:
                  memset(buffer_, 0x00, bufsize_);
                break;
            }
        }

        void SSD1306::update()
        {
            uint8_t payload[] = {SET_COL_ADDR, 0, (uint8_t)(width_ - 1), SET_PAGE_ADDR, 0, (uint8_t)(pages_ - 1)};
            if (width_ == 64)
            {
                payload[1] += 32;
                payload[2] += 32;
            }

            for (size_t i = 0; i < sizeof(payload); ++i)
            {
                write(payload[i]);
            }

            *(buffer_ - 1) = 0x40;

            i2cBus_.writeBlocking(address_, buffer_ - 1, bufsize_ + 1);
        }

        bool SSD1306::write(uint8_t val)
        {
            uint8_t d[2] = {0x00, val};
            return i2cBus_.writeBlocking(address_, d, 2);
        }

        void SSD1306::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color color)
        {
            if (x1 > x2)
            {
                swap(&x1, &x2);
                swap(&y1, &y2);
            }

            if (x1 == x2)
            {
                if (y1 > y2)
                {
                    swap(&y1, &y2);
                }

                for (int32_t i = y1; i <= y2; ++i)
                {
                    drawPixel(x1, i, color);
                }
                return;
            }

            float m = (float)(y2 - y1) / (float)(x2 - x1);

            for (int32_t i = x1; i <= x2; ++i)
            {
                float y = m * (float)(i - x1) + (float)y1;
                drawPixel(i, (uint32_t)y, color);
            }
        }

        void SSD1306::drawPixel(uint32_t x, uint32_t y, Color color)
        {
            if (x >= width_ || y >= height_)
            {
                return;
            }

            
            switch(color)
            {
                case Color::Black:
                    buffer_[x + width_ * (y >> 3)] &= ~(0x1 << (y & 0x07));
                break;

                case Color::White:
                    buffer_[x + width_ * (y >> 3)] |= 0x1 << (y & 0x07);
                break;
            }
        }

        void SSD1306::powerOff()
        {
            write(SET_DISP | 0x00);
        }

        void SSD1306::powerOn()
        {
            write(SET_DISP | 0x01);
        }

        void SSD1306::contrast(uint8_t val)
        {
            write(SET_CONTRAST);
            write(val);
        }

        void SSD1306::invert(uint8_t inv)
        {
            write(SET_NORM_INV | (inv & 1));
        }

        void SSD1306::drawSquare(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color)
        {
            for (uint32_t i = 0; i < width; ++i)
            {
                for (uint32_t j = 0; j < height; ++j)
                {
                    drawPixel(x + i, y + j, color);
                }
            }
        }

        void SSD1306::drawEmptySquare(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color)
        {
            drawLine(x, y, x + width, y, color);
            drawLine(x, y + height, x + width, y + height, color);
            drawLine(x, y, x, y + height, color);
            drawLine(x + width, y, x + width, y + height, color);
        }

        void SSD1306::drawChar(uint32_t x, uint32_t y, uint32_t scale, char c, Color color, const cilo72::fonts::Font & font)
        {
            if (c < font.firstAscciiChar() || c > font.lastAscciiChar())
            {
                return;
            }

            uint32_t parts_per_line = (font.height() >> 3) + ((font.height() & 7) > 0);
            for (uint8_t w = 0; w < font.width(); ++w)
            { // width
                uint32_t pp = (c - font.firstAscciiChar()) * font.width() * parts_per_line + w * parts_per_line;
                for (uint32_t lp = 0; lp < parts_per_line; ++lp)
                {
                    uint8_t line = font.data()[pp];

                    for (int8_t j = 0; j < 8; ++j, line >>= 1)
                    {
                        if (line & 1)
                        {
                            drawSquare(x + w * scale, y + ((lp << 3) + j) * scale, scale, scale, color);
                        }
                    }
                    ++pp;
                }
            }
        }

        void SSD1306::drawString(uint32_t x, uint32_t y, uint32_t scale,const char *s, Color color, const cilo72::fonts::Font & font)
        {
            for (int32_t x_n = x; *s; x_n += (font.width() + font.spacingPerChar()) * scale)
            {
                drawChar(x_n, y, scale, *(s++), color, font);
            }
        }
    }
}
