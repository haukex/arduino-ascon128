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
#include "z85.hpp"

static constexpr uint8_t _z85_tbl[] PROGMEM = "0123456789abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ.-:+=^!/*?&<>()[]{}@%$#";

void z85_print(Print &out, const uint8_t* buffer, size_t len) {
  uint32_t n;
  while (len) {
    n=0;  // If less than 4 bytes left, treat the rest of the bytes as 0!
    memcpy(&n, buffer, len>4 ? 4 : len);
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    n = __builtin_bswap32(n);  // Arduino is little-endian, Z85 uses big-endian
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#error "Big endian unsupported - I don't have a big-endian system to test on (yet)"
#else
#error "Endianness unknown"
#endif
                out.write(pgm_read_byte(_z85_tbl + (n / 52200625) % 85 ));
                out.write(pgm_read_byte(_z85_tbl + (n / 614125  ) % 85 ));
    if (len> 1) out.write(pgm_read_byte(_z85_tbl + (n / 7225    ) % 85 ));
    if (len> 2) out.write(pgm_read_byte(_z85_tbl + (n / 85      ) % 85 ));
    if (len> 3) out.write(pgm_read_byte(_z85_tbl +  n             % 85 ));
    if (len<=4) break;
    len -= 4;
    buffer += 4;
  }
}
