/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include "cilo72/hw/uart.h"
#include <vector>
#include <cstring>

namespace cilo72
{
    namespace hw
    {
        namespace
        {
            struct UartInstance
            {
                uart_inst_t *instance;
                bool used;
            };
            std::vector<UartInstance> uartInstances_ = {{uart0, false}, {uart1, false}};
        }

        Uart::Uart(uint pin_uart_rx, uint pin_uart_tx, uint32_t baudrate, uint data_bits, uint stop_bits, uart_parity_t parity)
            : uart_(nullptr)
        {
            int instance = -1;
            if (pin_uart_tx == 0 and pin_uart_rx == 1)
            {
                instance = 0;
            }
            else if (pin_uart_tx == 4 and pin_uart_rx == 5)
            {
                instance = 1;
            }
            else if (pin_uart_tx == 8 and pin_uart_rx == 9)
            {
                instance = 1;
            }
            else if (pin_uart_tx == 12 and pin_uart_rx == 13)
            {
                instance = 0;
            }
            else if (pin_uart_tx == 16 and pin_uart_rx == 17)
            {
                instance = 0;
            }
            else
            {
                assert(0);
            }

            assert(instance >= 0);

            if (uartInstances_[instance].used == false)
            {
                uartInstances_[instance].used = true;
                uart_ = uartInstances_[instance].instance;
            }

            assert(uart_ != nullptr);

            uart_init(uart_, baudrate);

            // Set the TX and RX pins by using the function select on the GPIO
            // Set datasheet for more information on function select
            gpio_set_function(pin_uart_tx, GPIO_FUNC_UART);
            gpio_set_function(pin_uart_rx, GPIO_FUNC_UART);

            uart_set_fifo_enabled(uart_, true);
            uart_set_format(uart_, data_bits, stop_bits, parity);
        }

        void Uart::clear() const
        {
            uint8_t b;
            while(receive(&b, 1, 0) != 0);
        }

        uint32_t Uart::receive(char * rx, uint32_t len, uint32_t firstTimeout, uint32_t interByteTimeout, const std::function<bool(uint8_t *, uint32_t)>& cb) const
        {
            return receive((uint8_t*)rx, len, firstTimeout, interByteTimeout, cb);
        }

        uint32_t Uart::receive(char * rx, uint32_t len, uint32_t timeout, const std::function<bool(uint8_t *, uint32_t)>& cb) const
        {
            return receive((uint8_t*)rx, len, timeout, timeout, cb);
        }

        uint32_t Uart::receive(uint8_t * rx, uint32_t len, uint32_t timeout, const std::function<bool(uint8_t *, uint32_t)>& cb) const
        {
            return receive(rx, len, timeout, timeout, cb);
        }

        uint32_t Uart::receive(uint8_t * rx, uint32_t len, uint32_t firstTimeout, uint32_t interByteTimeout, const std::function<bool(uint8_t *, uint32_t)>& cb) const
        {
            uint32_t i = 0;
            bool ret   = true;
            uint32_t timeout = firstTimeout;
            for(uint32_t i=0; i<len; i++)
            {
              ret = uart_is_readable_within_us(uart_, timeout*1000);
              if(ret) 
              {
                  uart_read_blocking(uart_, rx+i, 1);
                  if(cb)
                  {
                    if(not cb(rx, i+1))
                    {
                        return i+1;
                    }
                  }
              }
              else
              {
                return i;
              }
              timeout = interByteTimeout;
            }
            
            return i;
        }

        void Uart::transmit(const char * s) const
        {
            transmit((const uint8_t*)s, strlen(s));
        }

        void Uart::transmit(const uint8_t *tx, uint32_t len) const
        {
            uart_write_blocking(uart_, tx, len);
        }
    }
}
