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
  namespace ic
  {
    /**
     * @class KY_040
     * A class for handling KY-040 rotary encoder.
     * \see <a href="https://sensorkit.joy-it.net/de/sensors/ky-040">Datasheet</a> 
     */
    class KY_040
    {
    public:
      /**
       * Constructor for KY_040.
       * @param pinSW The pin for detecting button press.
       * @param pinDT The pin for detecting rotation direction.
       * @param pinCLK The pin for detecting rotation.
       */
      KY_040(uint8_t pinSW, uint8_t pinDT, uint8_t pinCLK);

      /**
       * Check if the button is pressed.
       * @return True if the button is pressed, false otherwise.
       */
      bool isPressed() const;

      /**
       * Get the accumulated encoder counts since the last call of this function.
       * @return The accumulated encoder counts.
       */
      int32_t getDelta();

      /**
       * Get the number of counts per turn of the encoder.
       * @return The number of counts per turn.
       */
      int32_t getCountPerTurn() const;

    private:
      cilo72::hw::RepeatingTimer repeatingTimer_; ///< Timer used for debouncing.
      uint8_t pinSW_;                             ///< Pin for detecting button press.
      uint8_t pinDT_;                             ///< Pin for detecting rotation direction.
      uint8_t pinCLK_;                            ///< Pin for detecting rotation.
      bool sw_;                                   ///< Current button state.
      int8_t lastValue_;                          ///< Last rotation value.
      int32_t delta_;                             ///< Accumulated encoder counts.
      int32_t deltaSingle_;                       ///< Encoder counts for the last turn.
      cilo72::hw::Debounce debounceSW_;           ///< Debounce object for button press.

      void timerCallback();
    };
  }
}