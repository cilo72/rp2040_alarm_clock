/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/hw/gpiokey.h"

namespace cilo72
{
    namespace hw
    {
        GpioKey::GpioKey(uint8_t pinKey)
            : repeatingTimer_(100, [&]()
                              { GpioKey::timerCallback(); }),
                              pinKey_(pinKey),
                              pressed_(false)

        {
            gpio_init(pinKey_);
            gpio_set_dir(pinKey_, GPIO_IN);
            down_ = lastDown_ = debounceSW_.in(gpio_get(pinKey_));
        }

        void GpioKey::timerCallback()
        {
            down_ = not debounceSW_.in(gpio_get(pinKey_));

            if(lastDown_ == false and down_ == true)
            {
                pressed_ = true;
            }

            lastDown_ = down_;
        }

        bool GpioKey::isPressed() const
        {
            return down_;
        }

        bool GpioKey::pressed()
        {
          bool ret = pressed_;
          pressed_ = false;
          return ret;
        }

    }
}
