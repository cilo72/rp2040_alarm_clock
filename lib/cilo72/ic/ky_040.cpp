/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/ic/ky_040.h"

namespace cilo72
{
    namespace ic
    {
        KY_040::KY_040(uint8_t pinSW, uint8_t pinDT, uint8_t pinCLK)
            : repeatingTimer_(1000, [&]()
                              { KY_040::timerCallback(); }),
              pinSW_(pinSW), pinDT_(pinDT), pinCLK_(pinCLK), delta_(0), deltaSingle_(0)
        {
            gpio_init(pinSW_);
            gpio_set_dir(pinSW_, GPIO_IN);

            gpio_init(pinDT_);
            gpio_set_dir(pinDT_, GPIO_IN);

            gpio_init(pinCLK_);
            gpio_set_dir(pinCLK_, GPIO_IN);

            uint32_t pins = gpio_get_all();
            int8_t actual = 0;
            sw_ = debounceSW_.in(pins & (1 << pinSW_));

            if (pins & (1 << pinDT_))
            {
                actual = 3;
            }

            if (pins & (1 << pinCLK_))
            {
                actual ^= 1;
            }
            lastValue_ = actual;
        }

        void KY_040::timerCallback()
        {
            uint32_t pins;
            int8_t actual = 0;
            int8_t diff;

            pins = gpio_get_all();
            sw_ = pins & (1 << pinSW_);

            if (pins & (1 << pinDT_))
            {
                actual = 3;
            }

            if (pins & (1 << pinCLK_))
            {
                actual ^= 1;
            }

            diff = lastValue_ - actual;

            if (diff & 1)
            {                                   // bit 0 = value (1)
                lastValue_ = actual;            // store new as next last
                deltaSingle_ += (diff & 2) - 1; // bit 1 = direction (+/-)
                if (deltaSingle_ == 4)
                {
                    delta_++;
                    deltaSingle_ = 0;
                }
                else if (deltaSingle_ == -4)
                {
                    delta_--;
                    deltaSingle_ = 0;
                }
            }
        }

        int32_t KY_040::getDelta()
        {
            int32_t ret = delta_;
            delta_ = 0;
            return ret;
        }
    }
}
