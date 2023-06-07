/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "cilo72/hw/spi_bus.h"

namespace cilo72
{
    namespace hw
    {
        /**
         * @brief The SPIDevice class provides an interface to an SPI device.
         */
        class SPIDevice
        {
        public:
            /**
             * @brief Constructs an SPIDevice object with the given SPI bus and CSN pin.
             * @param spiBus The SPIBus object representing the SPI bus.
             * @param pin_spi_csn The CSN (chip select not) pin number.
             */
            SPIDevice(SPIBus &spiBus, uint8_t pin_spi_csn);

            /**
             * @brief Transfers data over the SPI bus.
             * @param tx The data to transmit.
             * @param rx The buffer to receive the data.
             * @param len The length of the data.
             */
            void xfer(const uint8_t *tx, uint8_t *rx, size_t len) const;

        private:
            SPIBus &spiBus_;
            uint8_t pin_spi_csn_;
            void csSelect() const;
            void csDeselect() const;
        };
    }
}
