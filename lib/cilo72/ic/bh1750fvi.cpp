/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/ic/bh1750fvi.h"
#include <math.h>

namespace cilo72
{
    namespace ic
    {
        BH1750FVI::BH1750FVI(const cilo72::hw::I2CBus &i2cBus)
        : i2cBus_(i2cBus)
        , addr_(0x23)
        , last_(0.0)
        {
            write(OpCode::PowerOn);
            write(OpCode::ContinuouslyHResolution_Mode);
        }

        bool BH1750FVI::write(OpCode opcode)
        {
            uint8_t b = static_cast<uint8_t>(opcode);
            return i2cBus_.writeBlocking(addr_, &b, sizeof(b));
        }

        bool BH1750FVI::read(double & value)
        {
            uint8_t v[2];
            bool ret = i2cBus_.readBlocking(addr_, v, sizeof(v));
            value = (double)((v[0] << 8) | (v[1] << 0)) / 1.2;
            return ret;
        }

        BH1750FVI::operator const double &()
        {
            return last_;
        }

        void BH1750FVI::update()
        {
            read(last_);
        }
    }
}
