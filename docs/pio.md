
```cmake

cmake_minimum_required(VERSION 3.19)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "~/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_pio_pwm C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
add_executable(pico_pio_pwm)

pico_generate_pio_header(pico_pio_pwm ${CMAKE_CURRENT_LIST_DIR}/pico_servo.pio)

target_sources(pico_pio_pwm PRIVATE main.c)

target_link_libraries(pico_pio_pwm PRIVATE
  pico_stdlib
  hardware_pio
  pico_stdlib 
  hardware_pwm
)

# enable usb output, disable uart output
pico_enable_stdio_usb(pico_pio_pwm 1)
pico_enable_stdio_uart(pico_pio_pwm 0)

pico_add_extra_outputs(pico_pio_pwm)
```

```c
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "pico_servo.pio.h"

#define MIN_DC 650
#define MAX_DC 2250
const uint SERVO_PIN = 16;

// Write `period` to the input shift register
void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}

// Write `level` to TX FIFO. State machine will copy this into X.
void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
    pio_sm_put_blocking(pio, sm, level);
}

int main() {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &pico_servo_pio_program);

    float freq = 50.0f; // servo except 50Hz
    uint clk_div = 64;  // make the clock slower

    pico_servo_pio_program_init(pio, sm, offset, clk_div, SERVO_PIN);

    uint cycles = clock_get_hz(clk_sys) / (freq * clk_div);
    uint32_t period = (cycles -3) / 3;  
    pio_pwm_set_period(pio, sm, period);

    uint level;
    int ms = (MAX_DC - MIN_DC) / 2;
    bool clockwise = false;

  while (true) {
    level = (ms / 20000.f) * period;
    pio_pwm_set_level(pio, sm, level);

    if (ms <= MIN_DC || ms >= MAX_DC) {
        clockwise = !clockwise;
    }

    if (clockwise) {
        ms -= 100;
    } else {
        ms += 100;
    }

    sleep_ms(500);
  }
}
```

```
; Side-set pin 0 is used for PWM output

.program pico_servo_pio
.side_set 1 opt

    pull noblock    side 0 ; Pull from FIFO to OSR if available, else copy X to OSR.
    mov x, osr             ; Copy most-recently-pulled value back to scratch X
    mov y, isr             ; ISR contains PWM period. Y used as counter.
countloop:
    jmp x!=y noset         ; Set pin high if X == Y, keep the two paths length matched
    jmp skip        side 1
noset:
    nop                    ; Single dummy cycle to keep the two paths the same length
skip:
    jmp y-- countloop      ; Loop until Y hits 0, then pull a fresh PWM value from FIFO

% c-sdk {

static inline void pico_servo_pio_program_init(PIO pio, uint sm, uint offset, uint clk_div, uint pin) {
   pio_gpio_init(pio, pin);
   pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
   pio_sm_config c = pico_servo_pio_program_get_default_config(offset);
   sm_config_set_sideset_pins(&c, pin);
   sm_config_set_clkdiv(&c, clk_div);
   pio_sm_init(pio, sm, offset, &c);
}
%}
```
