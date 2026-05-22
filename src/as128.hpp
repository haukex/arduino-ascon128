/**
 * Copyright (c) 2026 Hauke Daempfling at the IGB Berlin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once
#include <Arduino.h>

/** This function encrypts the given buffer using the Ascon-128 algorithm.
 * The IV is used as the "associated data" (unencrypted but checksummed data).
 * The ``output`` buffer **MUST** hold at least ``len+16`` bytes, and this is
 * also the size of the resulting output buffer. Don't forget to transmit the
 * IV (associated data) to the receiver before transmitting the encrypted output. */
void as128_encrypt(const uint8_t secret[16], const uint8_t iv[16], const uint8_t* buffer, size_t len, uint8_t* output);

/** This function decrypts the given buffer using the Ascon-128 algorithm, assuming the buffer
 * was generated with the following structure: the "associated data" is exactly the IV and is the
 * first 16 bytes of the buffer, and the tag is the last 16 bytes of the buffer. The ``output``
 * buffer **MUST** hold at least ``len-32`` bytes, and this is also the size of the resulting
 * output buffer. If decryption fails, returns false. */
bool as128_decrypt(const uint8_t secret[16], const uint8_t* buffer, size_t len, uint8_t* output);
