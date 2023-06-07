/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/hw/spi_bus.h"
#include "vector"

namespace cilo72
{
    namespace hw
    {
        namespace
        {
            struct SPIInstance
            {
                spi_inst_t *instance;
                bool used;
            };
            std::vector<SPIInstance> spiInstances_ = {{spi0, false}, {spi1, false}};
        }

        SPIBus::SPIBus(uint8_t pin_spi_sck, uint8_t pin_spi_rx, uint8_t pin_spi_tx)
            : spiInstance_(nullptr)
        {
            int instance = -1;
            if (pin_spi_sck == 2 and pin_spi_rx == 0 and pin_spi_tx == 3)
            {
                instance = 0;
            }
            else if (pin_spi_sck == 6 and pin_spi_rx == 4 and pin_spi_tx == 7)
            {
                instance = 0;
            }
            else if (pin_spi_sck == 10 and pin_spi_rx == 8 and pin_spi_tx == 11)
            {
                instance = 1;
            }
            else if (pin_spi_sck == 14 and pin_spi_rx == 12 and pin_spi_tx == 15)
            {
                instance = 1;
            }
            else if (pin_spi_sck == 18 and pin_spi_rx == 16 and pin_spi_tx == 19)
            {
                instance = 0;
            }
            else
            {
                assert(0);
            }

            assert(instance >= 0);

            if (spiInstances_[instance].used == false)
            {
                spiInstances_[instance].used = true;
                spiInstance_ = spiInstances_[instance].instance;
            }

            assert(spiInstance_ != nullptr);

            spi_init(spiInstance_, 1000 * 1000);
            spi_set_format(spiInstance_, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
            gpio_set_function(pin_spi_rx, GPIO_FUNC_SPI);
            gpio_set_function(pin_spi_sck, GPIO_FUNC_SPI);
            gpio_set_function(pin_spi_tx, GPIO_FUNC_SPI);
        }
    }
}
