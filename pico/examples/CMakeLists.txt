
LIST(APPEND examples
    # adc
    serial
    # i2c
    # fixpt
    # pwm
)

message(STATUS ">> {PICO_BOARD}")
if (PICO_CYW43_SUPPORTED) # set by PICO_BOARD=pico_w
    # if (NOT TARGET pico_cyw43_arch)
    #     message("Skipping Pico W examples as support is not available")
    # else()
    if (TARGET pico_cyw43_arch)
        message("Compiling for Pico W")
        include(secrets.cmake)
        LIST(APPEND pico_libs
            pico_cyw43_arch_lwip_threadsafe_background)
        LIST(APPEND examples
            udp)
    else()
        message("Skipping Pico W examples")
    endif()
endif()


foreach(app ${examples})
    add_executable(${app} ${app}.cpp)
    target_link_libraries(${app} PRIVATE ${PROJECT_NAME} ${pico_libs})

    # if Pico W using WiFi, add these
    if (TARGET pico_cyw43_arch)
        target_compile_definitions(${app} PRIVATE
            WIFI_SSID=\"${WIFI_SSID}\"
            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
            )

        target_include_directories(${app}
            PRIVATE
                ${CMAKE_CURRENT_LIST_DIR}
                ../src
        )
    endif()

    # Set stdio functions to use USB rather than the serial ports
    pico_enable_stdio_usb(${app} 1)
    pico_enable_stdio_uart(${app} 0)

    # create uf2 output --------------------------------------------------
    pico_add_extra_outputs(${app})

    # fix cleanup command ------------------------------------------------
    set_target_properties(${app}
        PROPERTIES
            ADDITIONAL_CLEAN_FILES
            "${app}.bin;${app}.hex;${app}.elf;${app}.dis;${app}.elf.map;${app}.uf2")

    # Show info after project bulid ---------------------------------------
    add_custom_command(
        TARGET ${app}
        POST_BUILD
        COMMAND echo -------------------------------------
                && picotool info ${app}.uf2 -a
                && echo -------------------------------------
    )
endforeach()