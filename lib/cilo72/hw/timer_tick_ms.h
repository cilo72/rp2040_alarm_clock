/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

/**
 * @file
 * @brief This header file contains the declaration of the TimerTick_ms class.
 */

#pragma once

#include <stdint.h>
#include "hardware/timer.h"

namespace cilo72
{
    namespace hw
    {
        /**
         * @brief The TimerTick_ms class provides a static method to get the current timer tick in milliseconds.
         */
        class TimerTick_ms
        {
        public:
            /**
             * @brief Get the current timer tick.
             * @return The current timer tick.
             */
            static uint32_t now()
            {
                return time_us_64() / 1000;
            }

        private:
            /**
             * @brief The constructor of TimerTick_ms class is deleted because it is a static class.
             */
            TimerTick_ms() = delete;
        };
    }
}