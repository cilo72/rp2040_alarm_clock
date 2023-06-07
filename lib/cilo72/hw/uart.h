/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <stdint.h>
#include <functional>
#include "pico/stdlib.h"
#include "hardware/uart.h"

namespace cilo72
{
    namespace hw
    {
        /**
         * @brief The Uart class provides an interface for communication with UART devices.
         *
         * This class allows transmitting and receiving data over the UART interface.
         */
        class Uart
        {
        public:
            /**
             * @brief Constructor for the Uart class.
             * @param pin_uart_rx The RX pin for the UART interface.
             * @param pin_uart_tx The TX pin for the UART interface.
             * @param baudrate The baudrate to use for communication.
             * @param data_bits The number of data bits to use for communication.
             * @param stop_bits The number of stop bits to use for communication.
             * @param parity The parity to use for communication.
             */
            Uart(uint pin_uart_rx, uint pin_uart_tx, uint32_t baudrate, uint data_bits, uint stop_bits, uart_parity_t parity);

            /**
             * @brief Clears the UART receive buffer.
             */
            void clear() const;

            /**
             * @brief Transmits data over the UART interface.
             * @param tx A pointer to the data to transmit.
             * @param len The number of bytes to transmit.
             */
            void transmit(const uint8_t *tx, uint32_t len) const;

            /**
             * @brief Transmits a string over the UART interface.
             * @param s The null-terminated string to transmit.
             */
            void transmit(const char *s) const;

            /**
             * @brief Receives data over the UART interface.
             * @param rx A pointer to the buffer to receive the data.
             * @param len The maximum number of bytes to receive.
             * @param firstTimeout The maximum time to wait for the first byte of data.
             * @param interByteTimeout The maximum time to wait between subsequent bytes of data.
             * @param cb An optional callback function to process the received data. Return true as long as you want to process more data.
             * @return The number of bytes received.
             */
            uint32_t receive(uint8_t *rx, uint32_t len, uint32_t firstTimeout, uint32_t interByteTimeout, const std::function<bool(uint8_t *, uint32_t)> &cb = nullptr) const;

            /**
             * @brief Receives data over the UART interface.
             * @param rx A pointer to the buffer to receive the data.
             * @param len The maximum number of bytes to receive.
             * @param timeout The maximum time to wait for the data.
             * @param cb An optional callback function to process the received data. Return true as long as you want to process more data.
             * @return The number of bytes received.
             */
            uint32_t receive(uint8_t *rx, uint32_t len, uint32_t timeout, const std::function<bool(uint8_t *, uint32_t)> &cb = nullptr) const;

            /**
             * @brief Receives data over the UART interface as a string.
             * @param rx A pointer to the buffer to receive the data.
             * @param len The maximum number of bytes to receive.
             * @param firstTimeout The maximum time to wait for the first byte of data.
             * @param interByteTimeout The maximum time to wait between subsequent bytes of data.
             * @param cb An optional callback function to process the received data. Return true as long as you want to process more data.
             * @return The number of bytes received.
             */
            uint32_t receive(char *rx, uint32_t len, uint32_t timeout, const std::function<bool(uint8_t *, uint32_t)> &cb = nullptr) const;

            /**
             * @brief Receives data over the UART interface.
             * @param rx A pointer to the buffer to receive the data.
             * @param len The maximum number of bytes to receive.
             * @param firstTimeout The maximum time to wait for the first byte of data.
             * @param interByteTimeout The maximum time to wait between subsequent bytes of data.
             * @param cb An optional callback function to process the received data. Return true as long as you want to process more data.
             * @return The number of bytes received.
             */
            uint32_t receive(char * rx, uint32_t len, uint32_t firstTimeout, uint32_t interByteTimeout, const std::function<bool(uint8_t *, uint32_t)>& cb) const;

        private:
            uart_inst_t *uart_; ///< Pointer to the underlying UART instance.
        };
    }
}
