#include <stdio.h>
#include <string>

#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include "picolib.hpp"

using namespace std;
constexpr uint32_t uart0_pin_tx = 16;
constexpr uint32_t uart0_pin_rx = 17;
constexpr uint32_t uart1_pin_tx = 4;
constexpr uint32_t uart1_pin_rx = 5;

// UART -----------------------------------------------------------
bi_decl(bi_2pins_with_func(uart0_pin_rx, uart0_pin_tx, GPIO_FUNC_UART));
bi_decl(bi_2pins_with_func(uart1_pin_rx, uart1_pin_tx, GPIO_FUNC_UART));

int main() {
  uint8_t buffer[128];
  string msg = "serial0 send message";

  stdio_init_all();
  wait_for_usb();

  Serial Serial0;
  Serial Serial1;

  // init(speed, port, tx, rx)
  Serial0.init(9600, 0, uart0_pin_tx, uart0_pin_rx);
  Serial1.init(9600, 1, uart1_pin_tx, uart1_pin_rx);

  while (1) {
    Serial0.write((uint8_t *)msg.data(), msg.size());
    sleep_ms(500);
    Serial1.read(buffer, sizeof(buffer));

    buffer[msg.size()] = '\0';
    string m((char *)buffer);
    printf("msg: %s\n", (char *)buffer);
    printf("msg: %s\n", m.c_str());
  }
}