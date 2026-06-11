#include <stdio.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

#include <zmk/split/central.h>

#define BATTERY_UART_NODE DT_CHOSEN(zmk_battery_uart)

static const struct device *const battery_uart = DEVICE_DT_GET(BATTERY_UART_NODE);

static void write_response(void) {
    uint8_t left = 0;
    uint8_t right = 0;
    char response[48];

    zmk_split_central_get_peripheral_battery_level(0, &left);
    zmk_split_central_get_peripheral_battery_level(1, &right);

    int length = snprintf(response, sizeof(response), "CRADIO_BATTERY L=%u R=%u\n", left, right);
    for (int i = 0; i < length; i++) {
        uart_poll_out(battery_uart, response[i]);
    }
}

static void battery_uart_thread(void *arg1, void *arg2, void *arg3) {
    uint8_t byte;
    bool request_pending = false;

    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    if (!device_is_ready(battery_uart)) {
        return;
    }

    while (true) {
        while (uart_poll_in(battery_uart, &byte) == 0) {
            request_pending = true;
            if (byte == '\n' || byte == '\r') {
                write_response();
                request_pending = false;
            }
        }

        if (request_pending) {
            write_response();
            request_pending = false;
        }

        k_sleep(K_MSEC(10));
    }
}

K_THREAD_DEFINE(cradio_battery_uart, 512, battery_uart_thread, NULL, NULL, NULL, 10, 0, 0);
