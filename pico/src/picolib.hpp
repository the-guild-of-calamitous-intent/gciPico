/**************************************\
 * The MIT License (MIT)
 * Copyright (c) 2023 Kevin Walchko
 * see LICENSE for full details
\**************************************/
#pragma once

#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include <cstdint>
// #include "hardware/gpio.h" // already in other headers
#include "tusb.h" // wait for USB

typedef uint32_t pin_t;

#include "adc.hpp"
#include "fifo.hpp"
#include "i2c.hpp"
#include "mutex.hpp"
#include "pwm.hpp"
#include "rtc.hpp"
#include "spi.hpp"
#include "time.hpp"
#include "uart.hpp"
#include "usb.hpp"
#include "watchdog.hpp"

static void wait_for_usb(uint32_t msec = 100) {
  while (!tud_cdc_connected()) {
    sleep_ms(msec);
  }
}