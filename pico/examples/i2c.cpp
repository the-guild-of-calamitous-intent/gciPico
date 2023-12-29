#include <stdio.h>
// #include <string>

#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include "picolib.hpp"

using namespace std;

// I2C ------------------------------------------------------------
bi_decl(bi_2pins_with_func(I2C0_SDA_PIN, I2C0_SCL_PIN, GPIO_FUNC_I2C));
bi_decl(bi_2pins_with_func(I2C1_SDA_PIN, I2C1_SCL_PIN, GPIO_FUNC_I2C));

int main() {
  stdio_init_all();
  wait_for_usb();

  TwoWire Wire0;
  Wire0.init(I2C_400KHZ, 0, I2C0_SDA_PIN, I2C0_SCL_PIN);

  TwoWire Wire1;
  Wire1.init(I2C_400KHZ, 1, I2C1_SDA_PIN, I2C1_SCL_PIN);

  while (1) {
    sleep_ms(500);
  }
}