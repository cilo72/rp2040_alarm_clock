/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/ic/ws2812.h"
#include "cilo72/hw/pio.h"
#include <algorithm>
#include "pico/time.h"
#include "ws2812.pio.h"

namespace cilo72
{
    namespace ic
    {
        WS2812::WS2812(uint32_t pin, uint32_t numberOPixels)
        : brightnessIndex_(brightnessMapLength_-1)
        {
            pio_ = cilo72::hw::Pio::get();
            uint offset = pio_add_program(pio_.pio, &ws2812_program);
            ws2812_program_init(pio_.pio, pio_.sm, offset, pin, 800000, false);

            pixels_.reserve(numberOPixels);
            for (uint32_t i = 0; i < numberOPixels; i++)
            {
                pixels_.push_back(urgb_u32(0, 0, 0));
            }
        }

        uint32_t WS2812::urgb_u32(uint8_t r, uint8_t g, uint8_t b)
        {
            return ((uint32_t)(r) << 8) |
                   ((uint32_t)(g) << 16) |
                   (uint32_t)(b);
        }

        void WS2812::set(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b)
        {
            if(pixel < pixels_.size())
            {
                pixels_[pixel] = urgb_u32(r, g, b);
            }
        }

        void WS2812::set(uint8_t r, uint8_t g, uint8_t b)
        {
            for(uint8_t pixel = 0; pixel < pixels_.size(); pixel++)
            {
                pixels_[pixel] = urgb_u32(r, g, b);
            }
        }

        void WS2812::update()
        {
            union rgb
            {
                uint8_t  bytes[4];
                uint32_t raw;
            };

            rgb r;
            sleep_us(50);
            for(auto pixel : pixels_)
            {
                r.raw = pixel;
                r.bytes[1] = (uint32_t(r.bytes[1]) * brightnessMap_[brightnessIndex_]) / 255;
                r.bytes[2] = (uint32_t(r.bytes[2]) * brightnessMap_[brightnessIndex_]) / 255;
                r.bytes[0] = (uint32_t(r.bytes[0]) * brightnessMap_[brightnessIndex_]) / 255;
                pio_sm_put_blocking(pio_.pio, pio_.sm, (r.raw) << 8u);
            }
            while(not pio_sm_is_tx_fifo_empty(pio_.pio, pio_.sm))
            {
                
            }
        }

        int8_t WS2812::setBrightness(int8_t level)
        {
            if(level >= brightnessMapLength_)
            {
              brightnessIndex_ = brightnessMapLength_-1;
            }
            else if(level < 0)
            {
              brightnessIndex_ = 0;
            }
            else
            {
              brightnessIndex_ = level;
            }
            return brightnessIndex_;
        }

        int8_t WS2812::incBrightness(int8_t inc)
        {
            return setBrightness(brightnessIndex_ + inc);
        }

        void WS2812::rotateRight()
        {
            std::rotate(pixels_.rbegin(), pixels_.rbegin() + 1, pixels_.rend());
        }

        void WS2812::rotateLeft()
        {
            std::rotate(pixels_.begin(), pixels_.begin() + 1, pixels_.end());
        }
    }
}
