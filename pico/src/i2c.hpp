/**************************************\
 * The MIT License (MIT)
 * Copyright (c) 2023 Kevin Walchko
 * see LICENSE for full details
\**************************************/
#pragma once

#include "hardware/i2c.h"

/*
Maybe change to:
static bool initialized0, initialized1
baud = i2c_bus_init(port,baud,sda,scl)
ok = i2c_bus_scan(port,ids[],ids_size)

have to maintain the initialized ... or how likely is
it that I will re-init a bus twice??
*/

// constexpr uint I2C0_SDA_PIN    = 4;
// constexpr uint I2C0_SCL_PIN    = 5;
// constexpr uint I2C1_SDA_PIN    = 14;
// constexpr uint I2C1_SCL_PIN    = 15;
constexpr uint32_t I2C_100KHZ      = 100 * 1000UL;
constexpr uint32_t I2C_400KHZ      = 400 * 1000UL;
constexpr uint32_t I2C_1MHZ        = 1000 * 1000UL;
constexpr uint32_t I2C_BUFFER_SIZE = 32;

#ifndef I2C_BUS_DEFINES
  #define I2C_BUS_DEFINES
constexpr bool I2C_HOLD_BUS    = true;
constexpr bool I2C_RELEASE_BUS = false;
#endif

// determine if the pins passed in are valid for I2C
constexpr uint32_t sda_valid[2] = { // i2c0, i2c1
    (1 << 0) | (1 << 4) | (1 << 8) | (1 << 12) | (1 << 16) | (1 << 20) |
        (1 << 24) | (1 << 28),
    (1 << 2) | (1 << 6) | (1 << 10) | (1 << 14) | (1 << 18) | (1 << 22) |
        (1 << 26)};
constexpr uint32_t scl_valid[2] = { // i2c0, i2c1
    (1 << 1) | (1 << 5) | (1 << 9) | (1 << 13) | (1 << 17) | (1 << 21) |
        (1 << 25) | (1 << 29),
    (1 << 3) | (1 << 7) | (1 << 11) | (1 << 15) | (1 << 19) | (1 << 23) |
        (1 << 27)};

enum ErrorI2C : uint32_t {
  NONE = 0,
  INVALID_SCL_PIN,
  INVALID_SDA_PIN,
  INVALID_PORT,
  ALREADY_INITIALIZED,
  UNKNOWN
};

class TwoWire {
  i2c_inst_t *i2c;
  // uint8_t buffer[I2C_BUFFER_SIZE];
  static bool initialized0;
  static bool initialized1;

public:
  TwoWire() {}
  ~TwoWire() {}

  uint32_t baud{0};

  bool init(uint baud, uint8_t port, uint32_t pin_sda, uint32_t pin_scl) {
    // uint ret;
    if (port == 0) {
      i2c = &i2c0_inst;
      if (initialized0 == false) baud = i2c_init(i2c, baud);
      initialized0 = true;
    }
    else if (port == 1) {
      i2c = &i2c1_inst;
      if (initialized1 == false) baud = i2c_init(i2c, baud);
      initialized1 = true;
    }
    else return false;

    // printf(">> i2c instance: %u buad: %u\n", port, baud);
    // printf(">> i2c SDA: %u SCL: %u\n", pin_sda, pin_scl);

    // i2c_init(i2c, baud);
    gpio_set_function(pin_sda, GPIO_FUNC_I2C);
    gpio_set_function(pin_scl, GPIO_FUNC_I2C);
    gpio_pull_up(pin_sda);
    gpio_pull_up(pin_scl);

    // must be constants, cannot be a variable since it is a macro
    // bi_decl(bi_2pins_with_func(pin_sda, pin_scl, GPIO_FUNC_I2C));

    return true;
  }

  // Scans the I2C bus and returns an array of found addresses
  // https://github.com/raspberrypi/pico-examples/blob/master/i2c/bus_scan/bus_scan.c
  bool scan_bus(uint8_t *array, size_t size) {
    size_t index = 0;
    int ret;
    uint8_t rxdata;
    for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
      // Skip over any reserved addresses.
      if ((addr & 0x78) == 0 || (addr & 0x78) == 0x78) continue;

      ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);
      if (ret >= 0) array[index++] = addr;
      if (index == size) return false;
    }

    return true;
  }

  // bool write(const uint8_t addr, const uint8_t reg, const uint8_t data) {
  //   uint8_t out[2]{reg, data};
  //   i2c_write_blocking(i2c, addr, out, 2, I2C_RELEASE_BUS);
  //   return true;
  // }

  // bool read(const uint8_t addr, const uint8_t reg, const uint8_t data_size,
  //           uint8_t *const data) {
  //   i2c_write_blocking(i2c, addr, &reg, 1, I2C_HOLD_BUS);
  //   int ret = i2c_read_blocking(i2c, addr, data, data_size, I2C_RELEASE_BUS);
  //   return (ret < 0) ? false : true;
  // }

  // inline size_t available() { return i2c_get_read_available(i2c); }
};

bool TwoWire::initialized0 = false;
bool TwoWire::initialized1 = false;

static uint32_t i2c_bus_init(const uint32_t port, const uint32_t baud,
                             const uint32_t pin_sda, const uint32_t pin_scl) {
  if (port != 0 && port != 1) return INVALID_PORT;
  if (!(sda_valid[port] & (1 << pin_sda))) return INVALID_SDA_PIN;
  if (!(scl_valid[port] & (1 << pin_scl))) return INVALID_SCL_PIN;

  gpio_set_function(pin_sda, GPIO_FUNC_I2C);
  gpio_set_function(pin_scl, GPIO_FUNC_I2C);
  gpio_pull_up(pin_sda);
  gpio_pull_up(pin_scl);
  // Make the I2C pins available to picotool
  // THIS  WON'T WORK, cannot use variables, only constants
  // bi_decl(bi_2pins_with_func(pin_sda, pin_scl, GPIO_FUNC_I2C));

  if (port == 0) return i2c_init(&i2c0_inst, baud);
  else if (port == 1) return i2c_init(&i2c1_inst, baud);

  return UNKNOWN;
}

// Scans the I2C bus and returns an array of found addresses. The max number
// of slave devices is 127 (7-bit addressing), but usually it is far less.
// The function will not exceed the size of the buffer either.
//
// return: number of devices found
//
// https://github.com/raspberrypi/pico-examples/blob/master/i2c/bus_scan/bus_scan.c
static uint8_t i2c_bus_scan(const uint32_t port, uint8_t *array, size_t size) {
  size_t index = 0;
  int ret;
  uint8_t rxdata;
  i2c_inst_t *i2c = (port == 0) ? &i2c0_inst : &i2c1_inst;

  for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
    // Skip over any reserved addresses.
    if ((addr & 0x78) == 0 || (addr & 0x78) == 0x78) continue;

    ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);
    if (ret >= 0) array[index++] = addr;
    if (index == size) return (uint8_t)index;
  }
  return (uint8_t)index;
}