/**************************************\
 * The MIT License (MIT)
 * Copyright (c) 2023 Kevin Walchko
 * see LICENSE for full details
\**************************************/
#pragma once

class UDP {
  public:
  UDP() {}
  ~UDP() {}

  bool init() {

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_USA)) {
      printf("failed to initialise\n");
      return false;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi: %s %s\n", WIFI_SSID, WIFI_PASSWORD);
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
      printf("FAILED: unable to connect to %s\n", WIFI_SSID);
      return 1;
    } else {
      printf("SUCCESS: connected to %s\n", WIFI_SSID);
    }

    pcb = udp_new();
    udp_bind(pcb, IP_ADDR_ANY, UDP_PORT);

    ip_addr_t addr;
    ipaddr_aton(BEACON_TARGET, &addr);
    // IP4_ADDR(&addr, 127, 0, 0, 1);
    udp_connect(pcb,&addr,UDP_PORT);

    return true;
  }

  uint32_t send(uint8_* data, const uint32_t size) {
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, size+1, PBUF_RAM);
    // char *req = (char *)p->payload;
    // memset(req, 0, BEACON_MSG_LEN_MAX+1);
    // snprintf(req, BEACON_MSG_LEN_MAX, "%d\n\r", counter);
    memset(p->payload, data, size);
    err_t er = udp_sendto(pcb, p, &addr, UDP_PORT);
    pbuf_free(p);

    if (er != ERR_OK) return 0;
    return size;
  }

  uint32_t recv() {}

  protected:
  struct udp_pcb*{nullptr};
};