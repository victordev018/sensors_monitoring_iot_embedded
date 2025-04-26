// inclusion libraries
#include "wifi.h"
#include "pico/cyw43_arch.h"

// implementations functions
void wifi_connect() {

    // initialize of chip
    if (cyw43_arch_init()) {
        printf("Wi-fi init failed.\n");
        return;
    }

    // enable wifi client mode
    cyw43_arch_enable_sta_mode();

    // trying connection
    printf("Connecting Wifi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Failed to connect.\n");
        return;
    }

    printf("Wifi conected!\n");
}