/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

/**
 * @file
 * @brief This header file contains the declaration of the Pio class.
 */

#pragma once

#include "hardware/pio.h"
#include <vector>

namespace cilo72
{
  namespace hw
  {
    class Pio
    {
    public:
      struct Instance
      {
        PIO pio; //< PIO reference
        uint sm; //< Statemachine number.
      };

      /**
       * @brief Get an unused PIO.
       * If no unused PIO is available, an assertion occurs.
       */
      static const Instance get();

    private:
      Pio(PIO pio);
      std::vector<bool> smUsed_;
      PIO pio_;
    };
  }
}