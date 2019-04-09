#ifndef __CONFIG_H____
#define __CONFIG_H____

#define CONFIG_WIFI_SSID "<your wifi SSID>"
#define CONFIG_WIFI_PASSWORD "<your wifi password>"

#define CONFIG_SWARM_HOST "swarm-gateways.net"
#define CONFIG_SWARM_PORT 443
#define CONFIG_SWARM_SSL true

// Enter here your private key, one hex digit at a time:
#define CONFIG_PRIVATE_KEY \
    { 0xBB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

#endif
