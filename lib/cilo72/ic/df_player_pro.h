/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <stdint.h>
#include "cilo72/hw/uart.h"

namespace cilo72
{
    namespace ic
    {
        /**
         * @brief Controls a Fermion DFPlayer Pro.
         * \see <a href="https://wiki.dfrobot.com/DFPlayer_PRO_SKU_DFR0768">Datasheet</a>
         */
        class DfPlayerPro
        {
        public:
            enum class PlayMode
            {
                REPEAT_ONE_SONG = 1,
                REPEAT_ALL = 2,
                PLAY_ONE_AND_PAUSE = 3,
                PLAY_RANDOMLY = 4,
                REPEAT_ALL_IN_FOLDER = 5
            };

            /**
             * @brief Constructor that initializes the UART interface.
             * @param uart The UART interface used to communicate with the DFPlayer Pro.
             */
            DfPlayerPro(const cilo72::hw::Uart &uart);

            /**
             * @brief Tests the DFPlayer Pro module.
             *
             * @return true if the test is successful, false otherwise.
             */
            bool test() const;

            /**
             * @brief Skips to the next track.
             *
             * @return true if the command is successful, false otherwise.
             */
            bool next();

            /**
             * @brief Starts playing the current track.
             *
             * @return true if the command is successful, false otherwise.
             */
            bool play();

            /**
             * @brief Pauses the playback.
             *
             * @return true if the command is successful, false otherwise.
             */
            bool pause();

            /**
             * @brief Sets the volume level.
             *
             * @param level The desired volume level (0-30).
             * @return true if the command is successful, false otherwise.
             */
            bool setVolume(uint8_t level) const;

            /**
             * @brief Increases or decreases the volume level by the specified increment.
             *
             * @param increment The amount by which to increase or decrease the volume level.
             * @return true if the command is successful, false otherwise.
             */
            bool incVolume(int8_t increment) const;

            /**
             * @brief Gets the current volume level.
             *
             * @param level A pointer to the variable that will hold the volume level.
             * @return true if the command is successful, false otherwise.
             */
            bool getVolume(uint8_t *level) const;

            bool setPlayMode(PlayMode playMode);

            uint8_t maxVolume() const { return 30; } 

        private:
            /**
             * @brief Enumerates the possible playing modes.
             */
            enum class PlayingMode
            {
                Unknown, /**< The playing mode is unknown. */
                Playing, /**< The DFPlayer Pro is playing a track. */
                Pausing  /**< The DFPlayer Pro is paused. */
            };

            const cilo72::hw::Uart &uart_; /**< The UART interface used to communicate with the DFPlayer Pro. */
            PlayingMode playingMode_;      /**< The current playing mode. */

            /**
             * @brief Sends a command to the DFPlayer Pro and checks the response.
             *
             * @param cmd The command to send.
             * @return true if the command is successful, false otherwise.
             */
            bool xfer(const char *cmd) const;

            /**
             * @brief Sends a command to the DFPlayer Pro and reads the response.
             *
             * @param cmd The command to send.
             * @param response A pointer to the buffer that will hold the response.
             * @param responseMaxLength The maximum length of the response buffer.
             * @param responseLength A pointer to the variable that will hold the actual length of the response.
             * @return true if the command is successful, false otherwise.
             */
            bool xfer(const char *cmd, char *response, uint32_t responseMaxLength, uint32_t *responseLength) const;
        };

    }
}