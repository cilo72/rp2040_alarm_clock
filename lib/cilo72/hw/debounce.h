/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <stdint.h>

namespace cilo72
{
  namespace hw
  {
    /**
     * @brief A class for debouncing boolean inputs
     */
    class Debounce
    {
    public:
      /**
       * @brief Constructor for the Debounce class
       * @param count The number of consecutive inputs needed to change the output value
       */
      Debounce(uint32_t count = 20)
          : count_(count), value_(false), lastValue_(value_), state_(State::Init), countdown_(count)
      {
      }

      /**
       * @brief Process a new input value and update the debounced output value
       * @param value The new input value
       * @return The debounced output value
       */
      bool in(bool value)
      {
        switch (state_)
        {
        case State::Init:
          lastValue_ = value_ = value;
          if (value_)
          {
            state_ = State::High;
          }
          else
          {
            state_ = State::Low;
          }
          break;

        case State::High:
          if (value)
          {
            countdown_ = count_;
          }
          else
          {
            countdown_--;
            if (countdown_ == 0)
            {
              state_ = State::Low;
              value_ = false;
            }
          }
          break;

        case State::Low:
          if (not value)
          {
            countdown_ = count_;
          }
          else
          {
            countdown_--;
            if (countdown_ == 0)
            {
              state_ = State::High;
              value_ = true;
            }
          }
          break;
        }

        return value_;
      }

      /**
       * @brief Get the current debounced output value
       * @return The debounced output value
       */
      bool get() const { return value_; }

    private:
      /**
       * @brief The current state of the debouncing algorithm
       */
      enum class State
      {
        Init, /**< Initialization state */
        High, /**< High state */
        Low   /**< Low state */
      };
      uint32_t count_;     /**< The number of consecutive inputs needed to change the output value */
      bool value_;         /**< The current debounced output value */
      bool lastValue_;     /**< The previous input value */
      State state_;        /**< The current state of the debouncing algorithm */
      uint32_t countdown_; /**< The current countdown value */
    };
  }
}