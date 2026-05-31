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

#include <Ascon128.h>

/** This function encrypts the given buffer using the Ascon-128 algorithm in-place using the buffer
 * structure as follows.
 *
 * The buffer must be at least 16 bytes in size, in addition the plaintext to be encrypted.
 * ``len`` must be the size of the buffer in bytes and **MUST** be at least 16.
 *
 * The ``iv`` buffer must contain the IV. The IV is used as the "associated data", that is,
 * unencrypted but checksummed data. The caller is responsible for ensuring that IVs are not
 * re-used, and for transmitting the IV to the recipient before transmitting the buffer, so that
 * the message structure matches that expected by ``as128_decrypt``.
 *
 * The last 16 bytes of the buffer must not contain any data, as this part of the buffer will be
 * overwritten to store the "tag" (checksum).
 *
 * The first ``len-16`` bytes before the tag are the plaintext that will be encrypted in-place.
 */
void as128_encrypt(Ascon128 &cipher, const uint8_t secret[16], const uint8_t iv[16], void *buffer, size_t len);

/** This function decrypts the given buffer using the Ascon-128 algorithm in-place using the buffer
 * structure as follows.
 *
 * The buffer must be at least 32 bytes in size, in addition to the ciphertext to be decrypted.
 * ``len`` must be the size of the buffer in bytes.
 *
 * The first 16 bytes of the buffer must contain the IV. These bytes are not modified by this
 * function, they are only used as the "associated data", that is, unencrypted but checksummed data.
 * The caller is responsible for verifying that IVs are not re-used.
 *
 * The last 16 bytes of the buffer must contain the "tag" (checksum).
 *
 * The bytes between the first 16 and the last 16 bytes are the ciphertext that will be decrypted
 * in-place.
 *
 * Returns ``false`` if the decryption failed, that is, the buffer is too small or the tag could not
 * be verified.
 * **Warning:** In this case, the buffer will be invalid and the caller must discard it!
 */
bool as128_decrypt(Ascon128 &cipher, const uint8_t secret[16], void *buffer, size_t len);
