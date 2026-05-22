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
#include "hex.hpp"

static int8_t _hex_nib2int(uint8_t c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

bool hex_decode(uint8_t* buffer, const size_t len) {
  if (len%2) return false;
  for (size_t i=0; i<len; i+=2) {
    const int8_t upper = _hex_nib2int(buffer[i]);
    if (upper<0) return false;
    const int8_t lower = _hex_nib2int(buffer[i+1]);
    if (lower<0) return false;
    buffer[i/2] = (upper << 4) | lower;
  }
  return true;
}

static inline uint8_t _hex_dig(uint8_t nib)
  { return nib < 10 ? '0' + nib : 'a' + nib - 10; }

void hex_encode(uint8_t* buffer, size_t len) {
  while (len--) {
    const uint8_t b = buffer[len];
    buffer[len*2]   = _hex_dig((b >> 4) & 0xF);
    buffer[len*2+1] = _hex_dig( b       & 0xF);
  }
}

void hex_print(Print &out, const uint8_t* buffer, const size_t len) {
  for (size_t i=0; i<len; i++) {
    const uint8_t b = buffer[i];
    out.write(_hex_dig((b >> 4) & 0xF));
    out.write(_hex_dig( b       & 0xF));
  }
}
