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

#include "hex.h"
#include "Arduino.h"

uint8_t getVal(char c)
{
    if (c >= '0' && c <= '9')
        return (uint8_t)(c - '0');
    else
    {
        if (c >= 'A' && c <= 'F')
        {
            return (uint8_t)(c - 'A' + 10);
        }else{
            return (uint8_t)(c - 'a' + 10);
        }
    }
}

void fromHex(uint8_t *dst, const char *src, int len)
{
    for (int i = 0; i < len; i++)
    {
        uint8_t nib1=getVal(src[i * 2]);
        uint8_t nib2=getVal(src[i * 2+1]);
        dst[i] = (nib1 << 4) + nib2;
    }
}

void fromHexPrefix(uint8_t *dst, const char *src, int len){
    fromHex(dst+2, src,len);
}

void toHex(char *dst, const uint8_t *src, int len)
{
    for (int i = 0; i < len; i++)
    {
        uint8_t nib1 = (src[i] >> 4) & 0x0F;
        uint8_t nib2 = (src[i] >> 0) & 0x0F;
        dst[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
        dst[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
    }
    dst[len * 2] = '\0';
}

void toHexPrefix(char *dst, const uint8_t *src, int len){
    dst[0]='0';
    dst[1]='x';
    toHex(dst+2, src, len);
}