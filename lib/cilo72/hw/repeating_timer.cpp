/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "repeating_timer.h"
#include "pico/time.h"
#include "hardware/sync.h"
namespace cilo72
{
    namespace hw
    {
        namespace
        {
            bool timer_callback(repeating_timer_t *rt) 
            {
                RepeatingTimer * repetingTimer = (RepeatingTimer *)rt->user_data;
                repetingTimer->callback();
                return true;
            }
        }

        RepeatingTimer::RepeatingTimer(int64_t interval_us, const std::function<void()> & callback)
        : repeating_timer_( { 0 })
        ,callback_(callback)
         
        {
           uint32_t status = save_and_disable_interrupts();
          if(not add_repeating_timer_us(interval_us, timer_callback, this, &repeating_timer_))
          {
            assert(0);
          }
           restore_interrupts(status);
        }

        void RepeatingTimer::callback()
        {
            callback_();
        }
    }
}

