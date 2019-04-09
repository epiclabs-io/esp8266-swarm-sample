/**
 * MIT License
 *
 * Copyright (c) 2018 Epic Labs, epiclabs.io
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Arduino.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#include <ArduinoJson.h>
#include <time.h>
#include "ESP8266WiFi.h"
#include "Thermistor.h"
#include "config.h"
#include "ethers.h"
#include "feed.h"
#include "hex.h"
#include "wificonfig.h"

#ifndef __CONFIG_H____
#error "Create config.h out of config.h.template before compiling"
#endif

const uint8_t privateKey[ETHERS_PRIVATEKEY_LENGTH] = CONFIG_PRIVATE_KEY;

feed_t feed;
const char* topicName = "temperature";
Client client;
Thermistor t1(4100, 10000);

const char* swarmHost = CONFIG_SWARM_HOST;

double getTemperature() {
    int AR = analogRead(A0);

    int R_series = 4700;
    double V;
    V = AR / 1024.0 * 3.3;

    // V = R_series *3.3 / (R_th + R_series)
    // (R_th + R_series) = R_series * 3.3 / V
    // (R_th ) = R_series * 3.3 / V - R_series

    double R = R_series * 3.3 / V - R_series;

    double temp = t1.getTemperature(R);
    Serial.printf("Voltage: %g, Temperature = %g\n", V, temp);
    return temp;
}

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

    // Set WiFi to station mode and disconnect from an AP if it was previously
    // connected
    WiFi.mode(WIFI_STA);
    const char* ssid = CONFIG_WIFI_SSID;
    WiFi.begin(ssid, CONFIG_WIFI_PASSWORD);
    Serial.printf("\nConnecting to WiFi %s ...", ssid);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    char hexAddress[ETHERS_CHECKSUM_ADDRESS_LENGTH];
    ethers_privateKeyToAddress(privateKey, feed.user);
    ethers_addressToChecksumAddress(feed.user, hexAddress);

    Serial.printf("Ethereum Address: %s\n", hexAddress);

    memset(&feed.topic, 0, FEED_TOPIC_LENGTH);
    strcpy((char*)&feed.topic, topicName);
}

void loop() {
    digitalWrite(LED_BUILTIN, LOW);
    double temperature = getTemperature();

    StaticJsonBuffer<70> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["temperature"] = temperature;

    uint8_t data[root.measureLength() + 1];
    root.printTo((char*)data, sizeof(data));

    Topic topic;
    memset(&topic, 0, sizeof(topic));
    memcpy(&topic, "temperature", 11);

    Serial.println("Posting...");
    bool ok = feed_post_update(client, privateKey, topic, data, sizeof(data) - 1);
    Serial.printf("OK=%d\n", ok);

    if (!ok) {  // Flash the LED in case of error
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }

    digitalWrite(LED_BUILTIN, HIGH);
    delay(10000);
}