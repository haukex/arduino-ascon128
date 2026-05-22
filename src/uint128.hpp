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

#if !defined(__BYTE_ORDER__) || (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__)
#error "Endianness unknown or not supported"
#endif

/* If you only need a 32-bit IV, an alternative is the following, which though not "safe"
 * due to the strict aliasing rules of the C/C++ standards, GCC allows this. From its docs:
 * 'The practice of reading from a different union member than the one most recently written to
 * (called "type-punning") is common. Even with `-fstrict-aliasing`, type-punning is allowed in
 * C, provided the memory is accessed through the union type. In ISO C++, type-punning through
 * a union type is undefined behavior, but GCC supports it as an extension.'
 *
 * typedef union {
 *   uint8_t  bytes[16];
 *   uint32_t words[4];
 * } u128buf_t;
 * static_assert(sizeof(u128buf_t) == 16);  // make sure the compiler doesn't play tricks on us
 * u128buf_t iv = {0};                      // initializes *all* elements to 0
 */

// Arduino is little-endian so this struct works when using memcpy to a 16-byte buffer:
typedef struct { uint64_t lsb; uint64_t msb; } uint128_t;
// The memcpy-ed buffer can be decoded in Python by `int.from_bytes(buf16, byteorder='little')`

inline uint128_t& operator++(uint128_t& x)  // preincrement
  { if (!(++x.lsb)) x.msb++; return x; }

// the dummy `int` parameter distinguishes post- from pre-increment
inline uint128_t operator++(uint128_t& x, int)  // postincrement
  { uint128_t old = x; ++x; return old; }

inline bool operator==(const uint128_t& a, const uint128_t& b)
  { return (a.msb == b.msb) && (a.lsb == b.lsb); }

inline bool operator>(const uint128_t& a, const uint128_t& b)
  { return (a.msb > b.msb) || (a.msb == b.msb && a.lsb > b.lsb); }

inline bool operator<(const uint128_t& a, const uint128_t& b)
  { return (a.msb < b.msb) || (a.msb == b.msb && a.lsb < b.lsb); }
