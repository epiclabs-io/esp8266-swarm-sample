#include "config.h"

#ifndef __WIFI_CLIENT_SI_H__
#define __WIFI_CLIENT_SI_H__

#if CONFIG_SWARM_SSL
#include <WiFiClientSecure.h>
#define Client WiFiClientSecure
#else
#include <WiFiClient.h>
#define Client WiFiClient
#endif

#endif