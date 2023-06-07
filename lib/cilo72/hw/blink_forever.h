/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

/**
 * @file
 * @brief This header file contains the declaration of the BlinkForever class.
 * This is useful if you want to blink a status LED.
 */

#pragma once

#include <stdint.h>

namespace cilo72
{
    namespace hw
    {
        class BlinkForever
        {
        public:
            /**
             * @brief Blinks a pin forever using a PIO.
             * @param pin Pin.
             * @param freq Blinking frequency in Hz.
             */
            BlinkForever(uint32_t pin, uint32_t freq);
        };
    }
}