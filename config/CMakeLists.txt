if(CONFIG_SHIELD_CRADIO_DONGLE)
  target_sources(app PRIVATE
    src/cradio_battery_usb.c
    src/cradio_usb_only.c
  )
  zephyr_ld_options(-Wl,--wrap=bt_le_adv_start)
endif()
