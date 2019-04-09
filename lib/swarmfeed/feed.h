#ifndef __SWARM_FEEDS_H__
#define __SWARM_FEEDS_H__

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

#include <ArduinoJson.h>
#include "Arduino.h"
#include "../config/config.h"
#include "../config/wificonfig.h"
#include "ethers.h"
#include "hex.h"

const int FEED_TOPIC_LENGTH = 32;

typedef uint8_t Topic[FEED_TOPIC_LENGTH];

struct __attribute((__packed__)) feed_t {
    Topic topic;
    uint8_t user[ETHERS_ADDRESS_LENGTH];
};

struct __attribute((__packed__)) feed_update_t {
    uint8_t protocolVersion;
    uint8_t padding[7];
    feed_t feed;
    union {
        uint64_t time;
        struct
        {
            uint8_t lpad[7];
            uint8_t level;
        };
    };
};

bool feed_post_update(Client& client, const uint8_t* privateKey, const Topic topic, const uint8_t* data, const int dataLength);

#endif