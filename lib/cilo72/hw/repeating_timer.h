/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

/**
 * @file
 * @brief This header file contains the declaration of the RepeatingTimer class.
 */

#pragma once

#include "pico/stdlib.h"
#include <stdint.h>
#include <functional>

/**
 * @file
 * @brief This header file contains the declaration of the RepeatingTimer class.
 */

#pragma once

#include <stdint.h>
#include <functional>

namespace cilo72
{
    namespace hw
    {
        /**
         * @brief The RepeatingTimer class provides a repeating timer that executes a callback function at a fixed interval.
         */
        class RepeatingTimer
        {
        public:
            /**
             * @brief Constructs a RepeatingTimer object with the given interval and callback function.
             * @param interval_us The interval in microseconds.
             * @param callback The callback function to execute.
             */
            RepeatingTimer(int64_t interval_us, const std::function<void()> &callback);

            /**
             * @brief Executes the callback function.
             * Do not call it yourself.
             */
            void callback();

        private:
            repeating_timer_t repeating_timer_; /*< The repeating timer object. */
            std::function<void()> callback_;    /*< The callback function to execute. */
        };
    }
}