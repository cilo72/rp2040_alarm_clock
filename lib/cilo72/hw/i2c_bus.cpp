/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/hw/i2c_bus.h"
#include "pico/stdlib.h"
#include "vector"

namespace cilo72
{
    namespace hw
    {
        namespace
        {
          struct I2CInstance
          {
            i2c_inst_t * instance;
            bool used;
          };
          std::vector<I2CInstance> i2cInstances_ = { {i2c0, false} , {i2c1, false}};
        }

        I2CBus::I2CBus(uint pin_i2c_sda, uint pin_i2c_scl)
        : i2cInstance_(nullptr)
        {
            int instance = -1;
            if(pin_i2c_sda == 0 and pin_i2c_scl == 2)
            {
                instance = 0;
            }
            else if(pin_i2c_sda == 2 and pin_i2c_scl == 3)
            {
                instance = 1;
            }
            else if(pin_i2c_sda == 4 and pin_i2c_scl == 5)
            {
                instance = 0;
            }
            else if(pin_i2c_sda == 6 and pin_i2c_scl == 7)
            {
                instance = 1;
            }
            else if(pin_i2c_sda == 8 and pin_i2c_scl == 9)
            {
                instance = 0;
            }
            else if(pin_i2c_sda == 10 and pin_i2c_scl == 11)
            {
                instance = 1;
            }
            else if(pin_i2c_sda == 12 and pin_i2c_scl == 13)
            {
                instance = 0;
            }
            else if(pin_i2c_sda == 14 and pin_i2c_scl == 15)
            {
                instance = 1;
            }
            else if(pin_i2c_sda == 16 and pin_i2c_scl == 17)
            {
                instance = 0;
            }
            else if(pin_i2c_sda == 18 and pin_i2c_scl == 19)
            {
                instance = 1;
            }
            else if(pin_i2c_sda == 20 and pin_i2c_scl == 21)
            {
                instance = 0;
            }
            else if(pin_i2c_sda == 26 and pin_i2c_scl == 27)
            {
                instance = 1;
            }
            else
            {
                assert(0);
            }

            assert(instance >= 0);

            if(i2cInstances_[instance].used == false)
            {
                i2cInstances_[instance].used = true;
                i2cInstance_ = i2cInstances_[instance].instance;
            }

            assert(i2cInstance_ != nullptr);

            i2c_init(i2cInstance_, 400 * 1000);
            gpio_set_function(pin_i2c_sda, GPIO_FUNC_I2C);
            gpio_set_function(pin_i2c_scl, GPIO_FUNC_I2C);
            gpio_pull_up(pin_i2c_sda);
            gpio_pull_up(pin_i2c_scl);
        }

        bool I2CBus::writeBlocking(uint8_t addr, const uint8_t * src, size_t len) const
        {
            int ret = i2c_write_blocking(i2cInstance_, addr, src, len, false);
            return ret == (int)len;
        }

        bool I2CBus::readBlocking(uint8_t addr, uint8_t * dst, size_t len) const
        {
            int ret = i2c_read_blocking(i2cInstance_, addr, dst, len, false);
            return ret == (int)len;
        }

        bool I2CBus::xfer(uint8_t addr, const uint8_t * src, size_t lenSrc, uint8_t * dst, size_t lenDst) const
        {
            int retW = i2c_write_blocking(i2cInstance_, addr, src, lenSrc, true);
            int retR = i2c_read_blocking(i2cInstance_, addr, dst, lenDst, false);

            return (retW == (int)lenSrc) and (retR == (int)lenDst);
        }

    }
}
