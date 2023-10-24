
#pragma once


#include "hardware/adc.h"

constexpr uint A0         = 0;
constexpr uint A1         = 1;
constexpr uint A2         = 2;
constexpr uint A3         = 3;
constexpr float ADC_SCALE = 3.3f / (1 << 12);

class ADC {
  const uint pin;
  static bool initialized;

public:
  ADC(const uint pin) : pin(pin) {}
  ~ADC() {}

  bool init() {
    if (pin <= 3) {
      if (initialized == false) adc_init();
      initialized = true;

      // Make sure GPIO is high-impedance, no pullups etc
      adc_gpio_init(pin + 26);
      return true;
    }
    return false;
  }

  uint16_t read_raw() {
    adc_select_input(pin);
    return adc_read();
  }

  inline float read(const float scale=ADC_SCALE) { return scale * read_raw(); }
};

bool ADC::initialized = false;