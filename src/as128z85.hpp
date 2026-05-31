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

/** This function encrypts the given buffer using the Ascon-128 algorithm.
 *
 * The IV is used as the "associated data", that is, unencrypted but checksummed data, and it is
 * output before the encrypted data is output. After the encrypted data is output, the tag
 * (checksum) is also output. The result is directly written to the given stream in 16-byte blocks
 * with the Z85 encoding.
 *
 * The total number of bytes written to the output is ``len + 32 + (len + 35) / 4``
 * (assuming integer arithmetic).
 *
 * ``crypt_buf`` is the scratch buffer that will be used for encryption. If the input buffer is not
 * needed after encryption, ``crypt_buf`` may be the same as ``buffer``, though in this case of
 * course the buffer **MUST** hold (at least) the required 16 bytes!
 *
 * This function is an alternative to ``as128_encrypt`` for memory-constrained systems.
 */
void as128_encrypt_print_z85(Ascon128 &cipher, Print& out, const uint8_t secret[16],
    const uint8_t iv[16], const void *buffer, size_t len, uint8_t crypt_buf[16]);

/** A variant of ``as128_encrypt_print_z85`` where the 16-byte ``crypt_buf`` is allocated on the stack. */
void as128_encrypt_print_z85(Ascon128 &cipher, Print& out, const uint8_t secret[16],
    const uint8_t iv[16], const void *buffer, size_t len);
