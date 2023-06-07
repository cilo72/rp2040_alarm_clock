/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <stdint.h>

namespace cilo72
{
    namespace fonts
    {
        /**
         * @brief Abstract base class for font rendering.
         */
        class Font
        {
        public:
            /**
             * @brief Get the height of the font.
             * @return The height of the font in pixels.
             */
            virtual uint8_t height() const = 0;

            /**
             * @brief Get the width of the font.
             * @return The width of the font in pixels.
             */
            virtual uint8_t width() const = 0;

            /**
             * @brief Get the spacing between characters.
             * @return The spacing between characters in pixels.
             */
            virtual uint8_t spacingPerChar() const = 0;

            /**
             * @brief Get the ASCII code of the first character in the font.
             * @return The ASCII code of the first character in the font.
             */
            virtual uint8_t firstAscciiChar() const = 0;

            /**
             * @brief Get the ASCII code of the last character in the font.
             * @return The ASCII code of the last character in the font.
             */
            virtual uint8_t lastAscciiChar() const = 0;

            /**
             * @brief Get a pointer to the font data.
             * @return A pointer to the font data.
             */
            virtual const uint8_t *data() const = 0;

        private:
        };

    }
}
