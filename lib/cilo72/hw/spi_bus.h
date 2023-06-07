/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

/**
 * @file
 * @brief This header file contains the declaration of the SPIBus class.
 */

#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

namespace cilo72
{
    namespace hw
    {
        /**
         * @brief The SPIBus class provides an interface to an SPI bus.
         */
        class SPIBus
        {
        public:
            /**
             * @brief Constructs an SPIBus object with the given SCK, RX, and TX pins.
             * The constructor selects the SPI instance according the pins. If the instance is already used, an assertion occurs.
             * @param pin_spi_sck The SCK pin number.
             * @param pin_spi_rx The RX pin number.
             * @param pin_spi_tx The TX pin number.
             */
            SPIBus(uint8_t pin_spi_sck, uint8_t pin_spi_rx, uint8_t pin_spi_tx);

            /**
             * @brief Gets the SPI instance used by this SPIBus object.
             * @return A pointer to the spi_inst_t struct representing the SPI instance.
             */
            spi_inst_t *instance() const { return spiInstance_; };

        private:
            spi_inst_t *spiInstance_; /**< The SPI instance used by this SPIBus object. */
        };
    }
}
