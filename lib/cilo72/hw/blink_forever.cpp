/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/hw/blink_forever.h"
#include "cilo72/hw/pio.h"
#include "blink.pio.h"
#include "hardware/clocks.h"

namespace cilo72
{
    namespace hw
    {
        BlinkForever::BlinkForever(uint32_t pin, uint32_t freq)
        {
            Pio::Instance i = Pio::get();
            uint offset = pio_add_program(i.pio, &blink_program);
            blink_program_init(i.pio, i.sm, offset, pin);
            pio_sm_set_enabled(i.pio, i.sm, true);
            i.pio->txf[i.sm] = (clock_get_hz(clk_sys) / (2 * freq)) - 3;
        }
    }
}
