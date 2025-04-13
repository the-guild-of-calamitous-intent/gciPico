#include <cstdio>

#include "picolib.hpp"

#include "pico/float.h"

using namespace std;

int main() {
  stdio_init_all();
  wait_for_usb();

  uint32_t a = float2fix(3.45, 11);

  while (1) {
    sleep_ms(500);
  }
}