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
         * @class SD2405
         * @brief Class for controlling a RTC SD2405.
         * 
         * \see <a href="https://raw.githubusercontent.com/DFRobot/Gravity-I2C-SD2405-RTC-Module/master/SD2405AL%20datasheet.pdf">Datasheet</a> 
         */
        class SD2405
        {
        public:
            /**
             * @brief Class representing a point in time with year, month, day, week, hour, minute, and second components.
             */
            class Time
            {
            public:
                /**
                 * @brief Default constructor initializing all components to 0.
                 */
                Time()
                    : year_(0), month_(0), day_(0), week_(0), hour_(0), minute_(0), second_(0)
                {
                }

                /**
                 * @brief Constructor initializing all components to the specified values.
                 * @param year The year component.
                 * @param month The month component.
                 * @param day The day component.
                 * @param week The week component.
                 * @param hour The hour component.
                 * @param minute The minute component.
                 * @param second The second component.
                 */
                Time(uint16_t year, uint8_t month, uint8_t day, uint8_t week, uint8_t hour, uint8_t minute, uint8_t second)
                    : year_(year), month_(month), day_(day), week_(week), hour_(hour), minute_(minute), second_(second)
                {
                }

                /**
                 * @brief Getter for the year component.
                 * @return The year component.
                 */
                uint16_t year() const { return year_; }

                /**
                 * @brief Getter for the month component.
                 * @return The month component.
                 */
                uint8_t month() const { return month_; }

                /**
                 * @brief Getter for the week component.
                 * @return The week component.
                 */
                uint8_t week() const { return week_; }

                /**
                 * @brief Getter for the day component.
                 * @return The day component.
                 */
                uint8_t day() const { return day_; }

                /**
                 * @brief Getter for the hour component.
                 * @return The hour component.
                 */
                int8_t hour() const { return hour_; }

                /**
                 * @brief Getter for the minute component.
                 * @return The minute component.
                 */
                int8_t minute() const { return minute_; }

                /**
                 * @brief Getter for the second component.
                 * @return The second component.
                 */
                int8_t second() const { return second_; }

                /**
                 * @brief Setter for the hour component.
                 * @param value The new value for the hour component.
                 */
                void setHour(int8_t value)
                {
                    if (value > 23)
                        hour_ = 23;
                    else if (value < 0)
                        hour_ = 0;
                    else
                        hour_ = value;
                }

                /**
                 * @brief Setter for the minute component.
                 * @param value The new value for the minute component.
                 */
                void setMinute(int8_t value)
                {
                    if (value > 59)
                        minute_ = 59;
                    else if (value < 0)
                        minute_ = 0;
                    else
                        minute_ = value;
                }

                /**
                 * @brief Setter for the second component.
                 * @param value The new value for the second component.
                 */
                void setSecond(int8_t value)
                {
                    if (value > 59)
                        second_ = 59;
                    else if (value < 0)
                        second_ = 0;
                    else
                        second_ = value;
                }

            private:
                uint16_t year_;  ///< The year component.
                uint8_t month_;  ///< The month component.
                uint8_t day_;    ///< The day component.
                uint8_t week_;   ///< The week component.
                uint8_t hour_;   ///< The hour component.
                uint8_t minute_; ///< The minute component.
                uint8_t second_; ///< The second component.
            };

            /**
             * @brief Enumeration of user-defined registers in RTC SD2405.
             */
            enum class User
            {
                Register00 = 0x14, /**< User-defined register 00. */
                Register01 = 0x15, /**< User-defined register 01. */
                Register02 = 0x16, /**< User-defined register 02. */
                Register03 = 0x17, /**< User-defined register 03. */
                Register04 = 0x18, /**< User-defined register 04. */
                Register05 = 0x19, /**< User-defined register 05. */
                Register06 = 0x1A, /**< User-defined register 06. */
                Register07 = 0x1B, /**< User-defined register 07. */
                Register08 = 0x1C, /**< User-defined register 08. */
                Register09 = 0x1D, /**< User-defined register 09. */
                Register10 = 0x1E, /**< User-defined register 10. */
                Register11 = 0x1F  /**< User-defined register 11. */
            };

            /**
             * @brief Constructs an instance of the SD2405 class.
             * @param i2cBus The I2C bus to use for communication.
             */
            SD2405(const cilo72::hw::I2CBus &i2cBus);

            /**
             * @brief Returns the current time as a Time object.
             * @return The current time.
             */
            Time time() const;

            /**
             * @brief Sets the current time.
             * @param time The time to set.
             */
            void setTime(const Time &time) const;

            /**
             * @brief Returns the alarm time as a Time object.
             * @return The alarm time.
             */
            Time alarm() const;

            /**
             * @brief Sets the alarm time.
             * @param time The time to set.
             */
            void setAlarm(const Time &time) const;

            /**
             * @brief Returns the byte value of the specified user register.
             * @param index The index of the user register to read.
             * @return The byte value of the user register.
             */
            uint8_t byte(User index) const;

            /**
             * @brief Sets the byte value of the specified user register.
             * @param index The index of the user register to set.
             * @param value The byte value to set.
             */
            void setByte(User index, uint8_t value) const;

        private:
            const cilo72::hw::I2CBus &i2cBus_; /**< The I2C bus to use for communication. */
            uint8_t addr_;                     /**< The address of the RTC. */

            /**
             * @brief Converts a decimal number to binary-coded decimal (BCD) format.
             * @param num The decimal number to convert.
             * @return The BCD representation of the decimal number.
             */
            uint8_t decTobcd(uint8_t num) const;

            /**
             * @brief Enables the writing of the time to the RTC.
             */
            void writeTimeOn(void) const;

            /**
             * @brief Disables the writing of the time to the RTC.
             */
            void writeTimeOff(void) const;
        };
    }
}
