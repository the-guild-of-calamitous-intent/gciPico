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
  while (1) {
    sleep_ms(100);
  }
}