/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

/**
 * @file Tmc5160.h
 *
 * @brief Header file for Trinamic TM1560 controller.
 *
 * This file contains the definitions for the Trinamic TM1560 controller class
 * and its member functions, which allow for control of the TM1560 via SPI.
 */

#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "cilo72/hw/spi_device.h"

namespace cilo72
{
    namespace ic
    {

        /**
         * @class Tmc5160
         *
         * @brief Class for controlling Trinamic TM1560 stepper motor driver.
         *
         * This class provides functions for controlling the Trinamic TM1560 stepper
         * motor driver via SPI. It includes functions for setting and retrieving
         * configuration settings, as well as functions for monitoring the status of
         * the driver.
         * \see <a href="https://www.trinamic.com/fileadmin/assets/Products/ICs_Documents/TMC5160A_datasheet_rev1.18.pdf">Datasheet</a>
         */
        class Tmc5160
        {
        public:
            struct Status
            {
                union
                {
                    struct
                    {
                        uint8_t resetFlag : 1;       ///< Reset flag
                        uint8_t driverError : 1;     ///< Driver error
                        uint8_t sg2 : 1;             ///< StallGuard2
                        uint8_t standstill : 1;      ///< Standstill
                        uint8_t velocityReached : 1; ///< Target velocity reached
                        uint8_t positionReached : 1; ///< Target position reached
                        uint8_t statusStopL : 1;     ///< Stop left
                        uint8_t statusStopR : 1;     ///< Stop right
                    };
                    uint8_t reg; ///< Register value
                };
            };

            /**
             * @brief Struct representing the GSTAT register on the Trinamic TM1560
             */
            struct Gstat
            {
                union
                {
                    /**
                     * @brief Struct representing the bit fields in the GSTAT register
                     */
                    struct
                    {
                        uint32_t reset : 1;   /**< Bit 0: Reset */
                        uint32_t drv_err : 1; /**< Bit 1: Driver Error */
                        uint32_t uv_cp : 1;   /**< Bit 2: Under Voltage or Charge Pump Error */
                        uint32_t dummy : 29;  /**< Bits 3-31: Reserved for Future Use */
                    };
                    uint32_t reg; /**< Raw register value */
                };
            };

            /**
             * @brief Struct representing the GCONF register in Trinamic TM1560
             */
            struct Gconf
            {
                union
                {
                    struct
                    {
                        uint32_t recalibrate : 1;            ///< Recalibrate flag
                        uint32_t faststandstill : 1;         ///< Fast standstill enable flag
                        uint32_t en_pwm_mode : 1;            ///< Enable PWM mode flag
                        uint32_t multistep_filt : 1;         ///< Multistep filter flag
                        uint32_t shaft : 1;                  ///< Shaft flag
                        uint32_t diag0_error : 1;            ///< Diagnostics 0 error flag
                        uint32_t diag0_otpw : 1;             ///< Diagnostics 0 overtemperature protection warning flag
                        uint32_t diag0_stall : 1;            ///< Diagnostics 0 stall detection flag
                        uint32_t diag1_stall : 1;            ///< Diagnostics 1 stall detection flag
                        uint32_t diag1_index : 1;            ///< Diagnostics 1 index flag
                        uint32_t diag1_onstate : 1;          ///< Diagnostics 1 on-state flag
                        uint32_t diag1_steps_skipped : 1;    ///< Diagnostics 1 steps skipped flag
                        uint32_t diag0_int_pushpull : 1;     ///< Diagnostics 0 internal push-pull flag
                        uint32_t diag1_poscomp_pushpull : 1; ///< Diagnostics 1 position compare push-pull flag
                        uint32_t small_hysteresis : 1;       ///< Small hysteresis enable flag
                        uint32_t stop_enable : 1;            ///< Stop enable flag
                        uint32_t direct_mode : 1;            ///< Direct mode flag
                        uint32_t test_mode : 1;              ///< Test mode flag
                        uint32_t dummy : 14;                 ///< Unused bits
                    };
                    uint32_t reg; ///< 32-bit register representing the GCONF register
                };
            };

            /**
             * @brief Struct that represents the input/output status of the TM1560 controller.
             */
            struct IoIn
            {
                /**
                 * @brief Union that contains the bitfield values for the register.
                 */
                union
                {
                    /**
                     * @brief Struct that contains the individual bitfields.
                     */
                    struct
                    {
                        uint32_t refl_step : 1;      /**< Reflection of STEP pin state */
                        uint32_t refr_dir : 1;       /**< Reflection of DIR pin state */
                        uint32_t encb_dcen_cfg4 : 1; /**< Encoder B DCEN config bit */
                        uint32_t enca_dcin_cfg5 : 1; /**< Encoder A DCIN config bit */
                        uint32_t drv_enn_cfg6 : 1;   /**< DRV_ENN configuration bit */
                        uint32_t enc_n_dco : 1;      /**< Encoder N/DCO configuration bit */
                        uint32_t sd_mode : 1;        /**< Step/Dir mode selection bit */
                        uint32_t swcomp_in : 1;      /**< Switch input bit */
                        uint32_t dummy : 16;         /**< Unused bits */
                        uint32_t version : 8;        /**< Version number */
                    };
                    uint32_t reg; /**< The 32-bit register value */
                };
            };

            /**
             * @brief Structure representing the software mode settings for Trinamic TM1560.
             */
            struct Sw_Mode
            {
                union
                {
                    /**
                     * @brief Bitfield structure representing individual software mode settings.
                     */
                    struct
                    {
                        uint32_t stop_l_enable : 1;    /**< Stop left enable bit. */
                        uint32_t stop_r_enable : 1;    /**< Stop right enable bit. */
                        uint32_t pol_stop_l : 1;       /**< Polarity of stop left signal bit. */
                        uint32_t pol_stop_r : 1;       /**< Polarity of stop right signal bit. */
                        uint32_t swap_lr : 1;          /**< Swap left and right signals bit. */
                        uint32_t latch_l_active : 1;   /**< Latch left active bit. */
                        uint32_t latch_l_inactive : 1; /**< Latch left inactive bit. */
                        uint32_t latch_r_active : 1;   /**< Latch right active bit. */
                        uint32_t latch_r_inactive : 1; /**< Latch right inactive bit. */
                        uint32_t en_latch_encoder : 1; /**< Enable latch encoder bit. */
                        uint32_t sg_stop : 1;          /**< Stallguard stop enable bit. */
                        uint32_t en_softstop : 1;      /**< Enable software stop bit. */
                        uint32_t dummy : 20;           /**< Dummy bits. */
                    };
                    uint32_t reg; /**< Register value. */
                };
            };

            /**
             * @brief Struct to control the ChopConf configuration of a Trinamic TM1560
             */
            struct ChopConf
            {
                union
                {
                    /**
                     * @brief Struct containing the bitfields of the ChopConf register
                     */
                    struct
                    {
                        uint32_t toff : 4;     /**< Time offset value */
                        uint32_t hstrt : 3;    /**< Hysteresis start value */
                        uint32_t hend : 4;     /**< Hysteresis end value */
                        uint32_t fd3 : 1;      /**< Fast decay 3 value */
                        uint32_t disfdcc : 1;  /**< Disable fast decay comparator value */
                        uint32_t rndtf : 1;    /**< Random TOFF time value */
                        uint32_t chm : 1;      /**< Chop mode value */
                        uint32_t tbl : 2;      /**< Time base value */
                        uint32_t vsense : 1;   /**< Sense voltage value */
                        uint32_t vhighfs : 1;  /**< VHIGH chopping frequency selection value */
                        uint32_t vhighchm : 1; /**< VHIGH_CHM chopping frequency selection value */
                        uint32_t sync0 : 1;    /**< Sync mode bit 0 value */
                        uint32_t sync1 : 1;    /**< Sync mode bit 1 value */
                        uint32_t sync2 : 1;    /**< Sync mode bit 2 value */
                        uint32_t sync3 : 1;    /**< Sync mode bit 3 value */
                        uint32_t mres : 4;     /**< Microstep resolution value */
                        uint32_t intpol : 1;   /**< Interpolation enable value */
                        uint32_t dedge : 1;    /**< Double edge step value */
                        uint32_t diss2g : 1;   /**< Short-to-ground protection disable value */
                        uint32_t dummy : 1;    /**< Dummy bit value */
                    };
                    uint32_t reg; /**< ChopConf register value */
                };
            };

            /**
             * @brief Struct that represents the IHOLD_IRUN register of the Trinamic TM1560.
             */
            struct IHold_IRun
            {
                union
                {
                    /**
                     * @brief Bit-field struct representing the contents of the register.
                     */
                    struct
                    {
                        uint32_t ihold : 5;      /**< Current level during motor hold. */
                        uint32_t dummy1 : 3;     /**< Dummy bits. */
                        uint32_t irun : 5;       /**< Current level during motor run. */
                        uint32_t dummy2 : 3;     /**< Dummy bits. */
                        uint32_t iholddelay : 4; /**< Delay time before reducing current after motor stop. */
                        uint32_t dummy : 12;     /**< Dummy bits. */
                    };
                    uint32_t reg; /**< The contents of the register. */
                };
            };

            /**
             * @brief Structure for PWM configuration of Trinamic TM1560.
             */
            struct PwmConf
            {
                union
                {
                    /**
                     * @brief Bitfield layout for PWM configuration.
                     */
                    struct
                    {
                        uint32_t pwm_ampl : 8;      ///< PWM amplitude (0-255).
                        uint32_t pwm_grad : 8;      ///< PWM gradient (0-255).
                        uint32_t pwm_freq : 2;      ///< PWM frequency (0-3).
                        uint32_t pwm_autoscale : 1; ///< PWM autoscale (0 or 1).
                        uint32_t pwm_symmetric : 1; ///< PWM symmetric (0 or 1).
                        uint32_t freewheel : 2;     ///< Freewheeling mode (0-3).
                        uint32_t dummy : 10;        ///< Dummy bits (not used).
                    };
                    uint32_t reg; ///< PWM configuration register value.
                };
            };

            /**
             * @brief Struct for controlling the RampStat register of the Trinamic TM1560.
             */
            struct RampStat
            {
                union
                {
                    struct
                    {
                        uint32_t status_stop_l : 1;     ///< Stop left status bit.
                        uint32_t status_stop_r : 1;     ///< Stop right status bit.
                        uint32_t status_latch_l : 1;    ///< Latch left status bit.
                        uint32_t status_latch_r : 1;    ///< Latch right status bit.
                        uint32_t event_stop_l : 1;      ///< Stop left event bit.
                        uint32_t event_stop_r : 1;      ///< Stop right event bit.
                        uint32_t event_stop_sg : 1;     ///< Stop stallguard event bit.
                        uint32_t event_pos_reached : 1; ///< Target position reached event bit.
                        uint32_t velocity_reached : 1;  ///< Target velocity reached event bit.
                        uint32_t position_reached : 1;  ///< Target position reached status bit.
                        uint32_t vzero : 1;             ///< Vzero crossing status bit.
                        uint32_t t_zerowait_active : 1; ///< TMC26x/TMC2130: t_zerowait still active.
                        uint32_t second_move : 1;       ///< TMC26x/TMC2130: second move ongoing.
                        uint32_t status_sg : 1;         ///< Stallguard status bit.
                        uint32_t dummy : 18;            ///< Dummy bits.
                    };
                    uint32_t reg; ///< The 32-bit register.
                };
            };

            /**
             * @brief Struct representing the DRV_STATUS register in Trinamic TM1560.
             */
            struct Drv_Status
            {
                union
                {
                    struct
                    {
                        uint32_t sg_result : 10; ///< StallGuard value.
                        uint32_t reserved1 : 5;  ///< Reserved bits.
                        uint32_t fsactive : 1;   ///< Flag indicating if the full-step active mode is set.
                        uint32_t cs_actual : 5;  ///< Actual current step value.
                        uint32_t reserved2 : 3;  ///< Reserved bits.
                        uint32_t stallGuard : 1; ///< Flag indicating if the StallGuard value is greater than the threshold.
                        uint32_t ot : 1;         ///< Flag indicating over-temperature.
                        uint32_t otpw : 1;       ///< Flag indicating over-temperature prewarning.
                        uint32_t s2ga : 1;       ///< Sensor 2 status.
                        uint32_t s2gb : 1;       ///< Sensor 2 status.
                        uint32_t ola : 1;        ///< Flag indicating over-current on motor winding A.
                        uint32_t olb : 1;        ///< Flag indicating over-current on motor winding B.
                        uint32_t stst : 1;       ///< Status bit for step/dir interface.
                    };
                    uint32_t reg; ///< Register value.
                };
            };

            /**
             * @brief Configuration structure for coolstep current regulation.
             */
            struct CoolConf
            {
                union
                {
                    struct
                    {
                        uint32_t semin : 4; /**< Minimum coolstep current. */
                        uint32_t reserved1 : 1;
                        uint32_t seup : 2; /**< Maximum slope for increasing coolstep current. */
                        uint32_t reserved2 : 1;
                        uint32_t semax : 4; /**< Maximum coolstep current. */
                        uint32_t reserved3 : 1;
                        uint32_t sedn : 2;      /**< Maximum slope for decreasing coolstep current. */
                        uint32_t seimin : 1;    /**< Enable minimum coolstep current. */
                        uint32_t threshold : 7; /**< Upper threshold for switching on coolstep current. */
                        uint32_t reserved4 : 1;
                        uint32_t sfilt : 1; /**< Enable coolstep hysteresis filter. */
                        uint32_t reserved5 : 7;
                    };
                    uint32_t reg; /**< Raw register value. */
                };
            };

            /**
             * @brief The ramp mode of the Trinamic TM1560.
             */
            enum class RampMode
            {
                POSITIONING = 0,      ///< Positioning ramp mode.
                VELOCITY_POSITIV = 1, ///< Velocity positiv ramp mode.
                VELOCITY_NEGATIV = 2, ///< Velocity negativ ramp mode.
                HOLD = 3              ///< Hold ramp mode.
            };
            /**
             * @brief Constructor for Tmc5160 class.
             *
             * @param spi          SPI device object for TM1560.
             * @param pin_enable   Pin number for TM1560 enable pin.
             * @param rsens        Sense resistor value (default: 75).
             * @param fclk         SPI clock frequency (default: 12000000).
             */
            Tmc5160(cilo72::hw::SPIDevice &spi, uint8_t pin_enable, uint32_t rsens = 75, uint32_t fclk = 12000000);

            /**
             * @brief Get the status of the TM1560 driver.
             *
             * @return Status of the TM1560 driver.
             */
            Status status();

            /**
             * @brief Get the value of the IoIn register.
             *
             * @param[out] value  The value of the IoIn register.
             *
             * @return The status of the operation.
             */
            Status ioin(IoIn &value);

            /**
             * @brief Set the value of the GCONF register.
             *
             * @param value  The value to set the GCONF register to.
             *
             * @return The status of the operation.
             */
            Status setGconf(const Gconf &value);

            /**
             * @brief Get the value of the GCONF register.
             *
             * @param[out] value  The value of the GCONF register.
             *
             * @return The status of the operation.
             */
            Status gconf(Gconf &value);

            /**
             * @brief Get the value of the GSTAT register.
             *
             * @param[out] value  The value of the GSTAT register.
             *
             * @return The status of the operation.
             */
            Status gstat(Gstat &value);

            /**
             * @brief Set the value of the RAMP_MODE register.
             *
             * @param value  The value to set the RAMP_MODE register to.
             *
             * @return The status of the operation.
             */
            Status setRampmode(const RampMode value);

            /**
             * @brief Get the value of the RAMP_MODE register.
             *
             * @param[out] value  The value of the RAMP_MODE register.
             *
             * @return The status of the operation.
             */
            Status rampmode(RampMode &value);

            /**
             * @brief Set the value of the XACTUAL register.
             *
             * @param value  The value to set the XACTUAL register to.
             *
             * @return The status of the operation.
             */
            Status setXactual(int32_t value);
            /**
             * @brief Gets the current value of the X actual register
             * @param[out] value The value of the register
             * @return The status of the operation
             */
            Status xactual(int32_t &value);

            /**
             * @brief Gets the current value of the V actual register
             * @param[out] value The value of the register
             * @return The status of the operation
             */
            Status vactual(uint32_t &value);

            /**
             * @brief Sets the value of the V start register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setVstart(uint32_t value);

            /**
             * @brief Sets the value of the A1 register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setA1(uint32_t value);

            /**
             * @brief Sets the value of the V1 register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setV1(uint32_t value);

            /**
             * @brief Sets the value of the AMax register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setAMax(uint32_t value);

            /**
             * @brief Sets the value of the VMax register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setVMax(uint32_t value);

            /**
             * @brief Sets the value of the DMax register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setDMax(uint32_t value);

            /**
             * @brief Sets the value of the D1 register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setD1(uint32_t value);

            /**
             * @brief Sets the value of the V stop register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setVStop(uint32_t value);
            /**
             * @brief Sets the value of the T zero wait register
             * @param value The new value for the register
             * @return The status of the operation
             */
            Status setTZeroWait(uint32_t value);

             /**
             * @brief Sets the target position of the motor
             *
             * @param value The target position value
             * @return The driver status
             */
            Status setXTarget(int32_t value);

            /**
             * @brief Retrieves the current target position of the motor
             *
             * @param value Reference to a variable for storing the target position value
             * @return The driver status
             */
            Status xTarget(int32_t &value);

            /**
             * @brief Sets the minimum duty cycle for the motor
             *
             * @param value The minimum duty cycle value
             * @return The driver status
             */
            Status setVDcMin(uint32_t value);

            /**
             * @brief Sets the switching mode for the motor
             *
             * @param value The switching mode value to set
             * @return The driver status
             */
            Status setSwMode(Sw_Mode value);

            /**
             * @brief Retrieves the current switching mode for the motor
             *
             * @param value Reference to a variable for storing the switching mode value
             * @return The driver status
             */
            Status swMode(Sw_Mode &value);

            /**
             * @brief Retrieves the current ramp status for the motor
             *
             * @param value Reference to a variable for storing the ramp status value
             * @return The driver status
             */
            Status rampStat(RampStat &value);

            /**
             * @brief Retrieves the current latched position of the motor
             *
             * @param value Reference to a variable for storing the latched position value
             * @return The driver status
             */
            Status xLatch(int32_t &value);
            /**
             * @brief Set the driver's CHOPCONF register
             *
             * @param value CHOPCONF register value to set
             * @return Status of the operation
             */
            Status setChopConf(ChopConf value);

            /**
             * @brief Get the driver's CHOPCONF register value
             *
             * @param value Output parameter for the CHOPCONF register value
             * @return Status of the operation
             */
            Status chopConf(ChopConf &value);

            /**
             * @brief Set the driver's IHOLD_IRUN register value
             *
             * @param value IHOLD_IRUN register value to set
             * @return Status of the operation
             */
            Status setIholdIRun(IHold_IRun value);

            /**
             * @brief Set the T_PWM_THRS register value
             *
             * @param value T_PWM_THRS register value to set
             * @return Status of the operation
             */
            Status setTpwmThrs(uint32_t value);

            /**
             * @brief Set the TCOOL_THRS register value
             *
             * @param value TCOOL_THRS register value to set
             * @return Status of the operation
             */
            Status setTcoolThrs(uint32_t value);

            /**
             * @brief Get the TSTEP register value
             *
             * @param value Output parameter for the TSTEP register value
             * @return Status of the operation
             */
            Status tstep(uint32_t &value);

            /**
             * @brief Sets the PWM configuration.
             *
             * @param value The PWM configuration value.
             * @return Status The status of the operation.
             */
            Status setPwmConf(PwmConf value);

            /**
             * @brief Sets the CoolStep configuration.
             *
             * @param value The CoolStep configuration value.
             * @return Status The status of the operation.
             */
            Status setCoolConf(CoolConf value);

            /**
             * @brief Reads the driver status.
             *
             * @param value The output value.
             * @return Status The status of the operation.
             */
            Status drv_Status(Drv_Status &value);

            /**
             * @brief Converts the current in Trinamic units.
             * @param current Current in mA to convert.
             * @param valid Flag to indicate whether the current is valid or not.
             * @return The current scale value in Trinamic units.
             */
            uint8_t irms2CurrentScale(uint16_t current, bool &valid);

            /**
             * @brief Converts dRPS value to acceleration in Trinamic units.
             * @param drps DRPS value to convert.
             * @param uSteps Microsteps per full step.
             * @param fSteps Full steps.
             * @return Acceleration in Trinamic units.
             */
            double drps2Atmc(double drps, uint16_t uSteps, uint16_t fSteps);

            /**
             * @brief Converts RPS value to velocity in Trinamic units.
             * @param rps RPS value to convert.
             * @param uSteps Microsteps per full step.
             * @param fSteps Full stepps.
             * @return Velocity in Trinamic units.
             */
            double rps2Vtmc(double rps, uint16_t uSteps, uint16_t fSteps);

        private:
            cilo72::hw::SPIDevice &spi_;
            uint8_t pin_enable_;
            uint8_t lastReadAddr_;
            uint32_t rsens_;
            uint32_t fclk_;

            Status xfer(uint8_t addr, uint8_t read, uint32_t tx, uint32_t *rx);
            Status writeRegister(uint8_t addr, uint32_t tx);
            Status readRegister(uint8_t addr, uint32_t *rx);
        };
    }

}