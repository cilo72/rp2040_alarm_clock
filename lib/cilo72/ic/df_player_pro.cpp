/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/ic/df_player_pro.h"
#include <stdio.h>

namespace cilo72
{
    namespace ic
    {
        DfPlayerPro::DfPlayerPro(const cilo72::hw::Uart & uart)
        : uart_(uart)
        , playingMode_(PlayingMode::Pausing)
        {
        }

        bool DfPlayerPro::test() const
        {
            return xfer("AT");
        }

        bool DfPlayerPro::next()
        {
            playingMode_ = PlayingMode::Playing;
            return xfer("AT+PLAY=NEXT");
        }

        bool DfPlayerPro::play()
        {
            if(playingMode_ == PlayingMode::Playing)
            {
                return true;
            }

            bool ret  = xfer("AT+PLAY=PP");

            if(playingMode_ == PlayingMode::Pausing)
            {
                playingMode_ = PlayingMode::Playing;
            }
            return ret;
        }

        bool DfPlayerPro::pause()
        {
            if(playingMode_ == PlayingMode::Pausing)
            {
                return true;
            }

            bool ret  = xfer("AT+PLAY=PP");

            if(playingMode_ == PlayingMode::Playing)
            {
                playingMode_ = PlayingMode::Pausing;
            }
            return ret;
        }

        bool DfPlayerPro::setVolume(uint8_t level) const
        {
            char buf[30];
            sprintf(buf, "AT+VOL=%i", level);
            return xfer(buf);
        }

        bool DfPlayerPro::incVolume(int8_t increment) const
        {
            char buf[30];            
            sprintf(buf, "AT+VOL=%+i", increment);
            return xfer(buf);
        }

        bool DfPlayerPro::getVolume(uint8_t * level) const
        {
            uint32_t responseLength = 0;
            char rsp[30];
            int l;
            bool ret = xfer("AT+VOL=?", rsp, sizeof(rsp), &responseLength);
            if(ret and sscanf(rsp, "VOL = [%d]\r\n", &l) == 1)
            {
                (*level) = l;
                return true;
            }
            else
            {
              return false;
            }
        }

        bool DfPlayerPro::setPlayMode(PlayMode playMode)
        {
            char buf[30];
            sprintf(buf, "AT+PLAYMODE=%i", static_cast<int>(playMode));
            return xfer(buf);
        }

        bool DfPlayerPro::xfer(const char * cmd) const
        {
            uint32_t responseLength = 0;
            char response[10] = {0};
            return xfer(cmd, response, sizeof(response), &responseLength);
        }

        bool DfPlayerPro::xfer(const char * cmd, char * response, uint32_t responseMaxLength, uint32_t * responseLength) const
        {
           bool endFound = false;
           uart_.clear();
           uart_.transmit(cmd);
           uart_.transmit("\r\n");
           (*responseLength) = uart_.receive(response, responseMaxLength, 100, 10, [&](uint8_t * d, uint32_t l) -> bool 
           { 
             if(l >= 2)
             {
                if(d[l-2] == '\r' and d[l-1] == '\n')
                {
                    endFound = true;
                    return false;
                }
                else
                {
                    return true;
                }
             }
             else
             {
                return true;
             }
           } );

           if(endFound)
           {
             return true;
           }
           else
           {
             return false;
           }
        }
    }
}
