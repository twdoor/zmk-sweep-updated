#include <stddef.h>

#include <zephyr/bluetooth/bluetooth.h>

/* The dongle uses BLE only for split peripherals; host output is USB-only. */
int __wrap_bt_le_adv_start(const struct bt_le_adv_param *param, const struct bt_data *ad,
                           size_t ad_len, const struct bt_data *sd, size_t sd_len) {
    return 0;
}
