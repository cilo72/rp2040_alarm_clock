/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <vector>
#include <stdint.h>
#include "cilo72/hw/pio.h"

namespace cilo72
{
    namespace ic
    {
        /**
         * @brief Class representing a WS2812 LED strip.
         *
         * This class allows controlling WS2812 LED strips via the PIO interface.
         * The class provides methods for setting individual pixels, rotating the pixel array,
         * adjusting brightness and updating the strip with the current pixel values.
         * 
         * \see <a href="https://cdn-shop.adafruit.com/datasheets/WS2812.pdf">Datasheet</a> 
         */
        class WS2812
        {
        public:
            /**
             * @brief Constructs a new WS2812 object.
             *
             * @param pin The PIO output pin to use for the strip.
             * @param numberOPixels The number of pixels in the strip.
             */
            WS2812(uint32_t pin, uint32_t numberOPixels);

            /**
             * @brief Updates the strip with the current pixel values.
             */
            void update();

            /**
             * @brief Sets the RGB values of a single pixel.
             *
             * @param pixel The index of the pixel to set.
             * @param r The red component of the pixel (0-255).
             * @param g The green component of the pixel (0-255).
             * @param b The blue component of the pixel (0-255).
             */
            void set(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b);

            /**
             * @brief Sets the RGB values of all pixels in the strip.
             *
             * @param r The red component of the pixels (0-255).
             * @param g The green component of the pixels (0-255).
             * @param b The blue component of the pixels (0-255).
             */
            void set(uint8_t r, uint8_t g, uint8_t b);

            /**
             * @brief Rotates the pixel array to the left.
             */
            void rotateLeft();

            /**
             * @brief Rotates the pixel array to the right.
             */
            void rotateRight();

            /**
             * @brief Sets the brightness level of the strip.
             *
             * @param level The brightness level (0 to 15).
             * @return The new brightness level.
             */
            int8_t setBrightness(int8_t level);

            /**
             * @brief Increases the brightness level of the strip.
             *
             * @param inc The amount to increase the brightness level.
             * @return The new brightness level.
             */
            int8_t incBrightness(int8_t inc);

        private:
            std::vector<uint32_t> pixels_; ///< The pixel values.
            cilo72::hw::Pio::Instance pio_;  ///< The PIO instance used to control the strip.
            int8_t brightnessIndex_;       ///< The index into the brightness map for the current brightness level.

            static constexpr int8_t brightnessMapLength_ = 16;                                                                              ///< The length of the brightness map.
            static constexpr uint32_t brightnessMap_[brightnessMapLength_] = {0, 2, 3, 4, 6, 8, 11, 16, 23, 32, 45, 64, 90, 128, 181, 255}; ///< The brightness map, mapping brightness levels to PWM duty cycle values.

            /**
             * Converts RGB values to a 32-bit unsigned integer in the format used by WS2812 LEDs.
             *
             * @param r the red value of the pixel (0-255)
             * @param g the green value of the pixel (0-255)
             * @param b the blue value of the pixel (0-255)
             * @return the 32-bit unsigned integer representation of the pixel color
             */
            uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
        };
    }
}