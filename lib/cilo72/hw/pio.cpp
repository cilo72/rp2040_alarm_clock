/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "pio.h"
#include "vector"

namespace cilo72
{
    namespace hw
    {
        Pio::Pio(PIO pio)
            : pio_(pio)
        {
            static constexpr int NUMBER_OF_SM = 4;

            smUsed_.reserve(NUMBER_OF_SM);
            for (int i = 0; i < NUMBER_OF_SM; i++)
            {
                smUsed_.push_back(false);
            }
        }

        const Pio::Instance Pio::get()
        {
            static std::vector<Pio> pios = {Pio(pio0), Pio(pio1)};
            Instance i;

            for (auto &pio : pios)
            {
                for (uint32_t sm = 0; sm < pio.smUsed_.size(); sm++)
                {
                    if (not pio.smUsed_[sm])
                    {
                        pio.smUsed_[sm] = true;
                        i.pio = pio.pio_;
                        i.sm = sm;
                        return i;
                    }
                }
            }
            assert(0);
            return i;
        }
    }
}
