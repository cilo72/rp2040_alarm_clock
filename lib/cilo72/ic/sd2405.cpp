/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/ic/sd2405.h"
#include <string.h>

namespace cilo72
{
    namespace ic
    {
        SD2405::SD2405(const cilo72::hw::I2CBus &i2cBus)
        : i2cBus_(i2cBus)
        , addr_(0x32)
        {
        }

        SD2405::Time SD2405::time() const
        {
            uint8_t tx[]  = {0x00};
            uint8_t rx[7] = {0};
            bool ret = i2cBus_.xfer(addr_, tx, sizeof(tx), rx, sizeof(rx));

            assert(ret);

	        for (int i = 0; i<7; i++)
	        {
		      if (i != 2)
              {
			    rx[i] = (((rx[i] & 0xf0) >> 4) * 10) + (rx[i] & 0x0f);
              }
		      else
		      {
			    rx[2] = (rx[2] & 0x7f);
			    rx[2] = (((rx[2] & 0xf0) >> 4) * 10) + (rx[2] & 0x0f);
		      }
	        }

            return Time(rx[6] + 2000, rx[5], rx[4], rx[3], rx[2], rx[1], rx[0]);
        }

        void SD2405::setTime(const Time & time) const
        {
           uint8_t tx[8] = {0};
	       writeTimeOn();

           tx[0] = 0x00;
           tx[1] = decTobcd(time.second());
           tx[2] = decTobcd(time.minute());
           tx[3] = decTobcd(time.hour()) | 0x80;
           tx[4] = decTobcd(time.week());
           tx[5] = decTobcd(time.day());
           tx[6] = decTobcd(time.month());
           tx[7] = decTobcd(time.year() % 100);
	
           bool ret = i2cBus_.writeBlocking(addr_, tx, sizeof(tx));
           assert(ret);

           writeTimeOff();
        }

        SD2405::Time SD2405::alarm() const
        {
            uint8_t tx[]  = {0x07};
            uint8_t rx[7] = {0};
            bool ret = i2cBus_.xfer(addr_, tx, sizeof(tx), rx, sizeof(rx));

            assert(ret);

	        for (int i = 0; i<7; i++)
	        {
		      if (i != 2)
              {
			    rx[i] = (((rx[i] & 0xf0) >> 4) * 10) + (rx[i] & 0x0f);
              }
		      else
		      {
			    rx[2] = (rx[2] & 0x7f);
			    rx[2] = (((rx[2] & 0xf0) >> 4) * 10) + (rx[2] & 0x0f);
		      }
	        }

            return Time(rx[6] + 2000, rx[5], rx[4], rx[3], rx[2], rx[1], rx[0]);
        }

        void SD2405::setAlarm(const Time & time) const
        {
           uint8_t tx[8] = {0};
	       writeTimeOn();

           tx[0] = 0x07;
           tx[1] = decTobcd(time.second());
           tx[2] = decTobcd(time.minute());
           tx[3] = decTobcd(time.hour()) | 0x80;
           tx[4] = decTobcd(time.week());
           tx[5] = decTobcd(time.day());
           tx[6] = decTobcd(time.month());
           tx[7] = decTobcd(time.year() % 100);
	
           bool ret = i2cBus_.writeBlocking(addr_, tx, sizeof(tx));
           assert(ret);

           writeTimeOff();
        }

        uint8_t SD2405::decTobcd(uint8_t num) const
        {
	      return ((num / 10 * 16) + (num % 10));
        }        

        void SD2405::writeTimeOn(void) const
        {
            uint8_t tx1[] = { 0x10, //Set the address for writing as 10H
                              0x80  //Set WRTC1=1
                             };

            i2cBus_.writeBlocking(addr_, tx1, sizeof(tx1));

            uint8_t tx2[] = { 0x0F, //Set the address for writing as OFH
                              0x84  //Set WRTC2=1,WRTC3=1
                             };

            i2cBus_.writeBlocking(addr_, tx2, sizeof(tx2));
        }

        void SD2405::writeTimeOff(void) const
        {
            uint8_t tx1[] = { 0x0F, //Set the address for writing as OFH
                              0x00, //Set WRTC2=0,WRTC3=0
                              0x00  //Set WRTC1=0
                             };

            i2cBus_.writeBlocking(addr_, tx1, sizeof(tx1));
        }

        uint8_t SD2405::byte(User index) const
        {
            uint8_t tx[1] = {0};
            uint8_t rx[1] = {0};

            tx[0] = static_cast<uint8_t>(index);

            bool ret = i2cBus_.xfer(addr_, tx, sizeof(tx), rx, sizeof(rx));

            assert(ret);

            return rx[0];
        }

        void SD2405::setByte(User index, uint8_t value)  const
        {
            uint8_t tx[2] = {0};
            tx[0] = static_cast<uint8_t>(index);
            tx[1] = value;
            bool ret = i2cBus_.writeBlocking(addr_, tx, sizeof(tx));
            assert(ret);
        }

    }
}
