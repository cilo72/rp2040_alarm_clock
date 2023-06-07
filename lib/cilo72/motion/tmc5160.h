/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "cilo72/ic/tmc5160.h"
#include "cilo72/core/scale.h"
#include "cilo72/core/range.h"

namespace cilo72
{
    namespace motion
    {
        /**
         * @brief TMC5160 driver class
         */
        class Tmc5160
        {
        public:
            /**
             * @brief Constructor
             * @param ic Tmc5160 IC object
             * @param uSteps Microsteps per step
             * @param fSteps Full steps per rotation
             * @param feedPerRotation Feed per rotation
             */
            Tmc5160(cilo72::ic::Tmc5160 &ic, uint16_t uSteps, uint16_t fSteps, double feedPerRotation);

            /**
             * @brief Enable or disable motor power
             * @param enable If true, enable power. If false, disable power.
             * @return True if successful. False otherwise.
             */
            bool power(bool enable);

            /**
             * @brief Set currents for motor holding and running
             * @param ihold Holding current
             * @param irun Running current
             * @return True if successful. False otherwise.
             */
            bool setCurrents(uint16_t ihold, uint16_t irun);

            /**
             * @brief Move to an absolute position
             * @param position Absolute position to move to
             * @param velocity Velocity of movement
             * @param acceleration Acceleration of movement
             * @param iStopped Current when stopped
             * @param iMoving Current when moving
             * @return True if successful. False otherwise.
             */
            bool moveAbs(int32_t position, int32_t velocity, int32_t acceleration, uint16_t iStopped, uint16_t iMoving);

            /**
             * @brief Move a relative distance
             * @param distance Distance to move
             * @param velocity Velocity of movement
             * @param acceleration Acceleration of movement
             * @param iStopped Current when stopped
             * @param iMoving Current when moving
             * @return True if successful. False otherwise.
             */
            bool moveRel(int32_t distance, int32_t velocity, int32_t acceleration, uint16_t iStopped, uint16_t iMoving);

            /**
             * @brief Move at a certain velocity
             * @param velocity Velocity of movement
             * @param acceleration Acceleration of movement
             * @param iStopped Current when stopped
             * @param iMoving Current when moving
             * @return True if successful. False otherwise.
             */
            bool moveVelocity(int32_t velocity, int32_t acceleration, uint16_t iStopped, uint16_t iMoving);

            /**
             * @brief Move to the home position
             * @param position Position to use as home
             * @param iStopped Current when stopped
             * @param iMoving Current when moving
             * @return True if successful. False otherwise.
             */
            bool homingHere(int32_t position, uint16_t iStopped, uint16_t iMoving);

            /**
             * @brief Soft stop movement
             * @param acceleration Acceleration of movement
             * @return True if successful. False otherwise.
             */
            bool softStop(int32_t acceleration);

            /**
             * @brief Check if motor power is on
             * @return True if power is on. False otherwise.
             */
            bool isPowerOn();

            /**
             * @brief Check if position has been reached
             * @return True if position has been reached. False otherwise.
             */
            bool isPositionReached();

            /**
             * @brief Set motor direction to be inverted or not
             * @param inverse If true, motor direction is inverted. If false, it is not.
             */
            void setInverseMotorDirection(bool inverse);

            /**
             * Convert TMC status to error.
             * @return True if TMC status indicates an error, false otherwise.
             */
            bool tmcStatus2Error();

        private:
            cilo72::ic::Tmc5160 &ic_; /**< The IC object to communicate with. */
            uint16_t uSteps_;         /**< The microstep resolution. */
            uint16_t fSteps_;         /**< The fullstep resolution. */

            cilo72::core::Scale *scalePosition_;     /**< Scaling factor for position. */
            cilo72::core::Scale *scaleVelocity_;     /**< Scaling factor for velocity. */
            cilo72::core::Scale *scaleAcceleration_; /**< Scaling factor for acceleration. */

            cilo72::core::Range<int32_t> rangeVelocityTmc;     /**< Range of TMC velocity. */
            cilo72::core::Range<int32_t> rangeAccelerationTmc; /**< Range of TMC acceleration. */

            /**
             * Perform homing operation to set the current position to the given positionTmc.
             * @param positionTmc The position to set the motor to in TMC units.
             */
            void tmcHomingHere(int32_t positionTmc);

            /**
             * Move the motor to an absolute or relative position.
             * @param position The target position in steps or millimeters, depending on the scale used.
             * @param velocity The target velocity in steps/s or mm/s, depending on the scale used.
             * @param acceleration The target acceleration in steps/s^2 or mm/s^2, depending on the scale used.
             * @param iStopped The stall guard threshold for when the motor is considered stopped.
             * @param iMoving The stall guard threshold for when the motor is considered moving.
             * @param abs True if the position is absolute, false if it is relative.
             * @return True if the move is successful, false otherwise.
             */
            bool moveAbsOrRel(int32_t position, int32_t velocity, int32_t acceleration, uint16_t iStopped, uint16_t iMoving, bool abs);

            /**
             * Move the motor to an absolute position.
             * @param positionTmc The target position in TMC units.
             * @param velocityTmc The target velocity in TMC units.
             * @param accelerationTmc The target acceleration in TMC units.
             */
            void tmcMoveAbs(int32_t positionTmc, int32_t velocityTmc, int32_t accelerationTmc);
        };
    }
}
