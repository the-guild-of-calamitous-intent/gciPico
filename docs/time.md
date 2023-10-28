# Time

- sleep
    - `sleep_us(uint64_t us)` and `sleep_ms(uint32_t ms)`
- timestamp
    - `absolute_time_t = get_absolute_time (void)`
    - `typedef struct { uint64_t _private_us_since_boot; } absolute_time_t;`
- alarm
- repeating_timer

## References

- [Pi Pico Docs](https://cec-code-lab.aps.edu/robotics/resources/pico-c-api/index.html)
- [pico_time](https://cec-code-lab.aps.edu/robotics/resources/pico-c-api/group__pico__time.html)
