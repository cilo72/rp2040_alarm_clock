/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/hw/spi_device.h"

namespace cilo72
{
    namespace hw
    {
        SPIDevice::SPIDevice(SPIBus & spiBus, uint8_t pin_spi_csn)
        : spiBus_(spiBus)
        , pin_spi_csn_(pin_spi_csn)
        {
          gpio_init(pin_spi_csn);
          gpio_put(pin_spi_csn, 1);
          gpio_set_dir(pin_spi_csn, GPIO_OUT);
        }

        void SPIDevice::xfer(const uint8_t * tx, uint8_t * rx, size_t len) const
        {
            csSelect();
            spi_write_read_blocking(spiBus_.instance(), tx, rx, len);	
            csDeselect();
        }

        void SPIDevice::csSelect() const
        {
          asm volatile("nop \n nop \n nop"); // FIXME
          gpio_put(pin_spi_csn_, 0);
          asm volatile("nop \n nop \n nop"); // FIXME
        }

        void SPIDevice::csDeselect() const
        {
          asm volatile("nop \n nop \n nop"); // FIXME
          gpio_put(pin_spi_csn_, 1);
          asm volatile("nop \n nop \n nop"); // FIXME
       }        
    }
}