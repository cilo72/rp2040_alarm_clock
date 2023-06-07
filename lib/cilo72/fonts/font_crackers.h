/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0

  This implementation is heavily inspired by https://github.com/daschr/pico-ssd1306.
*/

#pragma once

#include <stdint.h>
#include "cilo72/fonts/font.h"

namespace cilo72
{
    namespace fonts
    {

        /**
         * @brief The FontCrackers class represents a font type that can be used to display text.
         *
         * This class implements the Font interface and provides methods to access the properties of the font.
         * The font data is stored in a byte array.
         */
        class FontCrackers : public Font
        {
        public:
            virtual uint8_t height() const override;
            virtual uint8_t width() const override;
            virtual uint8_t spacingPerChar() const override;
            virtual uint8_t firstAscciiChar() const override;
            virtual uint8_t lastAscciiChar() const override;
            virtual const uint8_t *data() const override;
        };
    }
}
