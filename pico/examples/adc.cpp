#include <stdio.h>
// #include <string>

#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <picolib.hpp>

using namespace std;

int main() {
  stdio_init_all();
  wait_for_usb();

  ADC volts;

  // ADC
  volts.init(A2);

  while (1) {
    float v = volts.read();
    printf("volts: %f\n", v);
    sleep_ms(100);
  }
}