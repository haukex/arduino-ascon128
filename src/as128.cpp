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
#include "as128.hpp"

#include <Ascon128.h>

static Ascon128 cipher;  // uses approx. 60 bytes (see private fields in Ascon128.h)

void as128_encrypt(const uint8_t secret[16], const uint8_t iv[16], const uint8_t* buffer, const size_t len, uint8_t* output) {
  // note .setKey also resets internal state
  cipher.setKey(secret, 16);  // key size is always 16
  cipher.setIV(iv, 16);       // IV size is always 16
  cipher.addAuthData(iv, 16);
  cipher.encrypt(output, buffer, len);
  cipher.computeTag(output+len, 16);  // tag size is always 16
  cipher.clear();
}

bool as128_decrypt(const uint8_t secret[16], const uint8_t* buffer, const size_t len, uint8_t* output) {
  if (len<32) return false;
  cipher.setKey(secret, 16);
  cipher.setIV(buffer, 16);
  cipher.addAuthData(buffer, 16);  // buffer starts with IV as additional data
  cipher.decrypt(output, buffer+16, len-32);
  const bool rv = cipher.checkTag(buffer+len-16, 16);  // buffer ends with tag
  cipher.clear();
  return rv;
}
