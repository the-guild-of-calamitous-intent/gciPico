/**************************************\
 * The MIT License (MIT)
 * Copyright (c) 2023 Kevin Walchko
 * see LICENSE for full details
\**************************************/
#pragma once

// #include "pico/stdlib.h"
// #include "pico/binary_info.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

constexpr uint A0         = 0;
constexpr uint A1         = 1;
constexpr uint A2         = 2;
constexpr uint A3         = 3;
constexpr float ADC_SCALE = 3.3f / (1 << 12);

class ADC {
  uint32_t pin{0};

public:
  ADC() { adc_init(); }
  ~ADC() {}

  bool init(const uint32_t pin) {
    if (pin <= 3) {
      // Make sure GPIO is high-impedance, no pullups etc
      adc_gpio_init(pin + 26);
      this->pin = pin;
      return true;
    }
    return false;
  }

  uint16_t read_raw(void) {
    adc_select_input(pin);
    return adc_read();
  }

  inline float read(void) { return ADC_SCALE * read_raw(); }
};

// template<uint pin>
// bool ADC<pin>::initialized = false;
// bool ADC::initialized = false;

// static ADC adc;