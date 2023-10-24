
#pragma once
#include "hardware/uart.h"

constexpr uint UART0_TX_PIN = 0;
constexpr uint UART0_RX_PIN = 1;
constexpr uint UART1_TX_PIN = 8;
constexpr uint UART1_RX_PIN = 9;

class Serial {
  uart_inst_t *uart;

public:
  Serial() {}
  ~Serial() { uart_deinit(uart); }

  bool init(uint baudrate, uint8_t port, const uint pin_a, const uint pin_b) {
    if (port == 0) uart = uart0;
    else if (port == 1) uart = uart1;
    else return false;

    gpio_set_function(pin_a, GPIO_FUNC_UART);
    gpio_set_function(pin_b, GPIO_FUNC_UART);
    uart_init(uart, baudrate);
    return true;
  }

  bool available() { return uart_is_readable(uart); }
  int read() { return 0; }
  void write(const uint8_t *data, size_t size) {}
};