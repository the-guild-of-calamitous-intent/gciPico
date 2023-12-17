#include <stdio.h>
// #include <string>

#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include "adc.hpp"
#include "i2c.hpp"
#include "uart.hpp"
// #include "string.hpp"

// #include <float.h>
// #include "pico.h"
// #include "pico/bootrom/sf_table.h"
#include "pico/float.h"

using namespace std;

TwoWire Wire0;
TwoWire Wire1;

Serial Serial0;
Serial Serial1;

ADC volts(A2);

const uint LED_PIN = 25;

int main() {
  // string hi{"how now brown cow ... where art thou\n"};

  // bi_decl(bi_program_description("This is a test binary."));
  // bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  uint32_t a = float2fix(3.45,11);

  // ADC
  volts.init();

  Serial0.init(115200, 0, UART0_TX_PIN, UART0_RX_PIN);
  Serial1.init(1000000, 1, UART1_TX_PIN, UART1_RX_PIN);

  Wire0.init(I2C_400KHZ, 0, I2C0_SDA_PIN, I2C0_SCL_PIN);
  Wire1.init(I2C_400KHZ, 1, I2C1_SDA_PIN, I2C1_SCL_PIN);

  while (1) {
    float v = volts.read();
    printf("volts: %f\n", v);

    // gpio_put(LED_PIN, 0);
    // sleep_ms(500);
    // gpio_put(LED_PIN, 1);
    // puts(">> Hello World\n");
    // puts(hi.c_str());
    // sleep_ms(500);
  }
}