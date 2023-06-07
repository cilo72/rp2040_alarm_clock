/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

/**
 * @file
 * @brief This header file contains the declaration of the ElapsedTimer_ms class.
 */

#pragma once

#include <stdint.h>
#include "cilo72/hw/timer_tick_ms.h"

namespace cilo72
{
    namespace hw
    {
        /**
         * @brief The ElapsedTimer_ms class provides a timer that measures elapsed time in milliseconds.
         */
        class ElapsedTimer_ms
        {
        public:
            /**
             * @brief Constructs an ElapsedTimer_ms object with start time initialized to zero.
             */
            ElapsedTimer_ms()
                : start_(0)
            {
            }

            /**
             * @brief Starts the timer by setting the start time to the current timer tick.
             */
            void start()
            {
                start_ = TimerTick_ms::now();
            }

            /**
             * brief Gets the elapsed time since the timer started.
             * return The elapsed time in milliseconds.
             */
            uint32_t elapsed() const
            {
                return (TimerTick_ms::now() - start_);
            }

            /**
             * @brief Checks whether the timer has expired.
             * @param timeout The timeout in milliseconds.
             * @return True if the timer has expired, false otherwise.
             */
            bool hasExpired(uint32_t timeout)
            {
                return elapsed() > timeout;
            }

        private:
            uint32_t start_; /**< The start time of the timer in milliseconds. */
        };
    }
}