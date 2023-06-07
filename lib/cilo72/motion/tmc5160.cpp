/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/motion/tmc5160.h"
#include <math.h>

namespace cilo72
{
    namespace motion
    {
        Tmc5160::Tmc5160(cilo72::ic::Tmc5160 &ic, uint16_t uSteps, uint16_t fSteps, double feedPerRotation)
            : ic_(ic), uSteps_(uSteps), fSteps_(fSteps),
              rangeVelocityTmc(0, (1 << 23) - 512),
              rangeAccelerationTmc(0, (1 << 16) - 1)

        {
            uint8_t steps;
            cilo72::ic::Tmc5160::ChopConf chopconf;

            switch (uSteps_)
            {
            case 256:
                steps = 0;
                break;
            case 128:
                steps = 1;
                break;
            case 64:
                steps = 2;
                break;
            case 32:
                steps = 3;
                break;
            case 16:
                steps = 4;
                break;
            case 8:
                steps = 5;
                break;
            case 4:
                steps = 6;
                break;
            case 2:
                steps = 7;
                break;
            case 1:
                steps = 8;
                break;
            default:
                assert(0);
            }

            ic_.chopConf(chopconf);
            chopconf.mres = steps;
            chopconf.toff = 0;
            chopconf.hstrt = 4;
            chopconf.hend = 1;
            chopconf.tbl = 2;
            chopconf.chm = 0;
            ic_.setChopConf(chopconf);

            double f = (uSteps * fSteps) / feedPerRotation;
            scalePosition_ = new cilo72::core::ScaleNumber(f);
            scaleVelocity_ = new cilo72::core::ScaleNumber(ic_.rps2Vtmc(1.0, uSteps, fSteps) / feedPerRotation);
            scaleAcceleration_ = new cilo72::core::ScaleNumber(ic_.drps2Atmc(1.0, uSteps, fSteps) / feedPerRotation);
        }

        bool Tmc5160::power(bool enable)
        {
            cilo72::ic::Tmc5160::ChopConf chopconf;

            ic_.chopConf(chopconf);
            if (enable)
            {
                chopconf.toff = 3;
            }
            else
            {
                chopconf.toff = 0;
            }
            ic_.setChopConf(chopconf);

            return true;
        }

        bool Tmc5160::moveVelocity(int32_t velocity, int32_t acceleration, uint16_t iStopped, uint16_t iMoving)
        {
            bool ret = true;

            ret = setCurrents(iStopped, iMoving);
            if (ret)
            {
                cilo72::core::ScaleResult vTmc = scaleVelocity_->scale(abs(velocity));
                cilo72::core::ScaleResult aTmc = scaleAcceleration_->scale(acceleration);

                if (isPowerOn() == false)
                {
                    ret = false;
                }
                else if (rangeVelocityTmc.inRange(vTmc) == false)
                {
                    ret = false;
                }
                else if (rangeAccelerationTmc.inRange(aTmc) == false)
                {
                    ret = false;
                }
                else
                {
                    ret = tmcStatus2Error();
                }

                if (ret)
                {
                    ic_.setA1(0);
                    ic_.setV1(0);
                    ic_.setAMax(aTmc);
                    ic_.setVMax(vTmc);
                    ic_.setDMax(0);
                    ic_.setD1(0);
                    ic_.setVStop(0);

                    ic_.setRampmode(velocity > 0 ? cilo72::ic::Tmc5160::RampMode::VELOCITY_POSITIV : cilo72::ic::Tmc5160::RampMode::VELOCITY_NEGATIV);
                }
            }

            return ret;
        }

        bool Tmc5160::setCurrents(uint16_t ihold, uint16_t irun)
        {
            cilo72::ic::Tmc5160::IHold_IRun iholdirun;
            bool ret = true;

            bool validIhold = false;
            bool validRun = false;

            iholdirun.reg = 0;
            iholdirun.ihold = ic_.irms2CurrentScale(ihold, validIhold);
            iholdirun.irun = ic_.irms2CurrentScale(irun, validRun);
            iholdirun.iholddelay = 4;

            if (validIhold && validRun)
            {
                ic_.setIholdIRun(iholdirun);
            }
            else
            {
                ret = false;
            }

            return ret;
        }

        bool Tmc5160::isPowerOn()
        {
            cilo72::ic::Tmc5160::ChopConf chopconf;

            ic_.chopConf(chopconf);

            return chopconf.toff != 0;
        }

        bool Tmc5160::isPositionReached()
        {
            cilo72::ic::Tmc5160::Status status = ic_.status();

            return status.positionReached == 1;
        }

        bool Tmc5160::tmcStatus2Error()
        {
            bool ret = true;
            cilo72::ic::Tmc5160::Drv_Status drv_status;

            ic_.drv_Status(drv_status);

            if (drv_status.s2ga || drv_status.s2gb)
            {
                ret = false;
            }
            if (drv_status.otpw)
            {
                ret = false;
            }
            if (drv_status.ot)
            {
                ret = false;
            }

            return ret;
        }

        bool Tmc5160::homingHere(int32_t position, uint16_t iStopped, uint16_t iMoving)
        {
            bool ret = true;

            int32_t pTmc = scalePosition_->scale(position);

            ret = power(true);
            if (ret != true)
            {
                goto exit;
            }

            ret = setCurrents(iStopped, iMoving);
            if (ret != true)
            {
                goto exit;
            }

            tmcHomingHere(pTmc);
        exit:
            return ret;
        }

        void Tmc5160::tmcMoveAbs(int32_t positionTmc, int32_t velocityTmc, int32_t accelerationTmc)
        {
            ic_.setA1(accelerationTmc);
            ic_.setV1(velocityTmc);
            ic_.setAMax(accelerationTmc);
            ic_.setVMax(velocityTmc);
            ic_.setDMax(accelerationTmc);
            ic_.setD1(accelerationTmc);
            ic_.setVStop(10);
            ic_.setRampmode(cilo72::ic::Tmc5160::RampMode::POSITIONING);
            ic_.setXTarget(positionTmc);
        }

        void Tmc5160::tmcHomingHere(int32_t positionTmc)
        {
            ic_.setV1(0);
            ic_.setVMax(0);
            ic_.setXTarget(positionTmc);
            ic_.setXactual(positionTmc);
        }

        void Tmc5160::setInverseMotorDirection(bool inverse)
        {
            cilo72::ic::Tmc5160::Gconf gconf;

            ic_.gconf(gconf);
            gconf.shaft = inverse ? 1 : 0;
            ic_.setGconf(gconf);
        }

        bool Tmc5160::moveAbs(int32_t position, int32_t velocity, int32_t acceleration, uint16_t iStopped, uint16_t iMoving)
        {
            return moveAbsOrRel(position, velocity, acceleration, iStopped, iMoving, true);
        }

        bool Tmc5160::moveRel(int32_t distance, int32_t velocity, int32_t acceleration, uint16_t iStopped, uint16_t iMoving)
        {
            return moveAbsOrRel(distance, velocity, acceleration, iStopped, iMoving, false);
        }

        bool Tmc5160::softStop(int32_t acceleration)
        {
            bool ret = true;
            cilo72::core::ScaleResult aTmc = scaleAcceleration_->scale(acceleration);

            if (rangeAccelerationTmc.inRange(aTmc) == false)
            {
                ret = false;
            }

            if (ret)
            {
                ic_.setA1(0);
                ic_.setV1(0);
                ic_.setAMax(aTmc);
                ic_.setVMax(0);
                ic_.setDMax(0);
                ic_.setD1(0);
                ic_.setVStop(0);
                ic_.setRampmode(cilo72::ic::Tmc5160::RampMode::VELOCITY_POSITIV);
            }

            return ret;
        }

        bool Tmc5160::moveAbsOrRel(int32_t position, int32_t velocity, int32_t acceleration, uint16_t iStopped, uint16_t iMoving, bool abs)
        {
            bool ret = true;

            ret = setCurrents(iStopped, iMoving);
            if (ret == true)
            {
                int32_t pActualTmc = 0;
                if (abs == false)
                {
                    ic_.xactual(pActualTmc);
                }

                cilo72::core::ScaleResult pTmc = scalePosition_->scale(position) + pActualTmc;
                cilo72::core::ScaleResult vTmc = scaleVelocity_->scale(velocity);
                cilo72::core::ScaleResult aTmc = scaleAcceleration_->scale(acceleration);

                if (isPowerOn() == false)
                {
                    ret = false;
                }
                else if (rangeVelocityTmc.inRange(vTmc) == false)
                {
                    ret = false;
                }
                else if (rangeAccelerationTmc.inRange(aTmc) == false)
                {
                    ret = false;
                }
                else
                {
                    ret = tmcStatus2Error();
                }

                if (ret == true)
                {
                    tmcMoveAbs(pTmc, vTmc, aTmc);
                }
            }
            return ret;
        }
    }
}
