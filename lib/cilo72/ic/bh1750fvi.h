/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "cilo72/hw/i2c_bus.h"

namespace cilo72
{
    namespace ic
    {
        /**
         * @brief Class for controlling BH1750FVI light sensor via I2C interface.
         * \see <a href="https://www.mouser.com/datasheet/2/348/bh1750fvi-e-186247.pdf">Datasheet</a>
         */
        class BH1750FVI
        {
        public:
            /**
             * @brief Constructor for BH1750FVI class.
             * @param i2cBus The I2C bus to which the device is connected.
             */
            BH1750FVI(const cilo72::hw::I2CBus &i2cBus);

            /**
             * @brief Read the current light intensity from the sensor.
             * @param[out] value The measured light intensity in lux.
             * @return True if the read was successful, false otherwise.
             */
            bool read(double &value);

            operator const double &();

            void update();

        private:
            /**
             * @brief Enumeration of BH1750FVI opcodes.
             */
            enum class OpCode
            {
                PowerOff = 0x00,
                PowerOn = 0x01,
                ContinuouslyHResolution_Mode = 0x10,
                ContinuouslyHResolution_Mode2 = 0x11,
            };

            const cilo72::hw::I2CBus &i2cBus_; ///< The I2C bus to which the device is connected.
            uint8_t addr_;                     ///< The I2C address of the device.
            double last_;

            /**
             * @brief Write a command to the BH1750FVI.
             * @param opcode The opcode to write.
             * @return True if the write was successful, false otherwise.
             */
            bool write(OpCode opcode);
        };

    }
}