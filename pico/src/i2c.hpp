/**************************************\
 * The MIT License (MIT)
 * Copyright (c) 2023 Kevin Walchko
 * see LICENSE for full details
\**************************************/
#pragma once

#include "hardware/i2c.h"

constexpr uint I2C0_SDA_PIN    = 4;
constexpr uint I2C0_SCL_PIN    = 5;
constexpr uint I2C1_SDA_PIN    = 14;
constexpr uint I2C1_SCL_PIN    = 15;
constexpr uint I2C_100KHZ      = 100 * 1000;
constexpr uint I2C_400KHZ      = 400 * 1000;
constexpr bool I2C_HOLD_BUS    = true;
constexpr bool I2C_RELEASE_BUS = false;
constexpr uint I2C_BUFFER_SIZE = 32;


constexpr uint32_t sda_valid[2] = { // i2c0, i2c1
    (1 << 0) | (1 << 4) | (1 << 8) | (1 << 12) | (1 << 16) | (1 << 20) |
        (1 << 24) | (1 << 28),
    (1 << 2) | (1 << 6) | (1 << 10) | (1 << 14) | (1 << 18) | (1 << 22) |
        (1 << 26)};
constexpr uint32_t scl_valid[2] = { // i2c0, i2c1
  (1 << 1) | (1 << 5) | (1 << 9) | (1 << 13) | (1 << 17) | (1 << 21) | (1 << 25) | (1 << 29),
  (1 << 3) | (1 << 7) | (1 << 11) | (1 << 15) | (1 << 19) | (1 << 23) | (1 << 27)};

class TwoWire {
  i2c_inst_t *i2c;
  uint8_t buffer[I2C_BUFFER_SIZE];
  static bool initialized0;
  static bool initialized1;

public:
  TwoWire() {}
  ~TwoWire() {}

  bool init(uint baud, uint8_t port, uint pin_sda, uint pin_scl) {
    uint ret;
    if (port == 0) {
      i2c = &i2c0_inst;
      if (initialized0 == false) ret = i2c_init(i2c, baud);
      initialized0 = true;
    }
    else if (port == 1) {
      i2c = &i2c1_inst;
      if (initialized1 == false) ret = i2c_init(i2c, baud);
      initialized1 = true;
    }
    else return false;

    // printf(">> i2c instance: %u buad: %u\n", port, ret);
    // printf(">> i2c SDA: %u SCL: %u\n", pin_sda, pin_scl);

    i2c_init(i2c, baud);
    gpio_set_function(pin_sda, GPIO_FUNC_I2C);
    gpio_set_function(pin_scl, GPIO_FUNC_I2C);
    gpio_pull_up(pin_sda);
    gpio_pull_up(pin_scl);
    return true;
  }

  // inline void beginTransmission(uint8_t address) { addr = address; }
  // inline void requestFrom(uint8_t address) { addr = address; }
  // inline void endTransmission() {}

  bool write(const uint8_t addr, const uint8_t reg, const uint8_t data) {
    uint8_t out[2]{reg, data};
    i2c_write_blocking(i2c, addr, out, 2, I2C_RELEASE_BUS);
    return true;
  }

  bool read(const uint8_t addr, const uint8_t reg, const uint8_t data_size, uint8_t *const data) {
    i2c_write_blocking(i2c, addr, &reg, 1, I2C_HOLD_BUS);
    int ret = i2c_read_blocking(i2c, addr, data, data_size, I2C_RELEASE_BUS);
    return (ret < 0) ? false : true;
  }

  inline size_t available() { return i2c_get_read_available(i2c); }
};

bool TwoWire::initialized0 = false;
bool TwoWire::initialized1 = false;