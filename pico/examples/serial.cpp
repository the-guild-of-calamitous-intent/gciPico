#include <stdio.h>
// #include <string>

#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include "picolib.hpp"

using namespace std;

// UART -----------------------------------------------------------
bi_decl(bi_2pins_with_func(UART0_RX_PIN, UART0_TX_PIN, GPIO_FUNC_UART));
bi_decl(bi_2pins_with_func(UART1_RX_PIN, UART1_TX_PIN, GPIO_FUNC_UART));

int main() {

  stdio_init_all();
  wait_for_usb();

  Serial Serial0;
  Serial Serial1;

  // init(speed, port, tx, rx)
  Serial0.init(115200, 0, UART0_TX_PIN, UART0_RX_PIN);
  Serial1.init(1000000, 1, UART1_TX_PIN, UART1_RX_PIN);

  while (1) {
    // Serial0.write("serial0\n");
    sleep_ms(500);
  }
}