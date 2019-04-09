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

#include "feed.h"

bool feed_post_update(Client& client, const uint8_t* privateKey, const Topic topic, const uint8_t* data, const int dataLength) {
    uint8_t digestData[sizeof(feed_update_t) + dataLength];
    feed_update_t* update = (feed_update_t*)digestData;
    memset(digestData, 0, sizeof(digestData));

    ethers_privateKeyToAddress(privateKey, update->feed.user);
    memcpy(update->feed.topic, topic, sizeof(Topic));

    char hexAddress[ETHERS_CHECKSUM_ADDRESS_LENGTH];
    char hexTopic[FEED_TOPIC_LENGTH * 2 + 2 + 1];
    ethers_addressToChecksumAddress(update->feed.user, hexAddress);
    toHexPrefix(hexTopic, (uint8_t*)topic, FEED_TOPIC_LENGTH);

    const char* host = CONFIG_SWARM_HOST;

    if (!client.connect(host, CONFIG_SWARM_PORT)) {
        return false;
    }

    /*
  if (client.verify(fingerprint, host))
  {
    Serial.println("certificate matches");
  }
  else
  {
    Serial.println("certificate doesn't match");
  }
*/

    String url;
    url = url + "/bzz-feed:/?user=" + hexAddress + "&topic=" + hexTopic + "&meta=1";
    Serial.println(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + CONFIG_SWARM_HOST + "\r\n" +
                 "User-Agent: ESP8266\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("request sent");
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
        if (line == "\r") {
            Serial.println("headers received");
            break;
        }
    }

    DynamicJsonBuffer jsonBuffer(400);

    // Parse JSON object
    JsonObject& root = jsonBuffer.parseObject(client);
    if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return false;
    }

    // Extract values
    Serial.println(F("Response:"));
    Serial.println(root["feed"]["topic"].as<char*>());
    Serial.println(root["feed"]["user"].as<char*>());
    Serial.println(root["epoch"]["time"].as<long>());
    Serial.println(root["epoch"]["level"].as<uint8_t>());

    update->time = root["epoch"]["time"].as<uint32_t>();
    update->level = root["epoch"]["level"].as<uint8_t>();
    memcpy(digestData + sizeof(feed_update_t), data, dataLength);

    char digestDataHex[sizeof(digestData) * 2 + 2 + 1];
    toHex(digestDataHex, digestData, sizeof(digestData));
    Serial.print("Digest data:");
    Serial.println(digestDataHex);

    uint8_t digest[ETHERS_KECCAK256_LENGTH];
    ethers_keccak256(digestData, sizeof(digestData), digest);

    uint8_t signature[ETHERS_SIGNATURE_LENGTH];
    ethers_sign(privateKey, digest, signature);

    char signatureHex[sizeof(signature) * 2 + 2 + 1];
    toHexPrefix(signatureHex, signature, sizeof(signature));

    const char* hexUser = root["feed"]["user"].as<char*>();

    url = "/bzz-feed:/?user=";
    url = url + hexUser + "&topic=" + hexTopic + "&time=" + uint32_t(update->time);
    url = url + "&level=" + update->level;
    url = url + "&signature=" + signatureHex;

    Serial.print("POST: connecting to ");
    Serial.println(host);
    if (!client.connect(host, CONFIG_SWARM_PORT)) {
        Serial.println("connection failed for POST");
        return 0;
    }

    Serial.printf(
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: ESP8266\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n",
        url.c_str(), host, dataLength);

    client.printf(
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: ESP8266\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n",
        url.c_str(), host, dataLength);

    client.write_P((const char*)data, dataLength);

    String line;
    if (client.connected()) {
        line = client.readStringUntil('\n');
        //client.stop();
        if (line.indexOf("200") > 0) {
            return true;
        }
    }

    Serial.println(line);
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
    }
    return false;
}