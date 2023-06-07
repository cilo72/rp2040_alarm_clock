/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/
 
#pragma once

#include "pico/stdlib.h"
#include "cilo72/hw/repeating_timer.h"
#include "cilo72/hw/debounce.h"

namespace cilo72
{
  namespace hw
  {
    /**
     * @class KY_040
     * A class for handling KY-040 rotary encoder.
     */
    class GpioKey
    {
    public:
      /**
       * Constructor for KY_040.
       * @param pinSW The pin for detecting button press.
       * @param pinDT The pin for detecting rotation direction.
       * @param pinCLK The pin for detecting rotation.
       */
      GpioKey(uint8_t pinKey);

      /**
       * Check if the button is pressed.
       * @return True if the button is pressed, false otherwise.
       */
      bool isPressed() const;

      operator const bool & () const 
      { 
        return down_; 
      }

      bool pressed();

    private:
      cilo72::hw::RepeatingTimer repeatingTimer_; ///< Timer used for debouncing.
      uint8_t pinKey_;                             ///< Pin for detecting button press.
      bool down_;                                   ///< Current button state.
      bool lastDown_;
      cilo72::hw::Debounce debounceSW_;           ///< Debounce object for button press.
      bool pressed_;

      void timerCallback();
    };
  }
}