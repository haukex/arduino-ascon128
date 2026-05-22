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

/** Decode the buffer as a string of hex characters *in place!*
 * The length must be divisible by two.
 * The length of the resulting buffer will be the length divided by two.
 * If decoding the string fails, returns false. */
bool hex_decode(uint8_t* buffer, size_t len);

/** Encode the buffer as a string of hex characters *in place!*
 * **WARNING:** The actual size of the available memory in the buffer *must*
 * be at least ``len*2``, and this is the length of the resulting buffer. */
void hex_encode(uint8_t* buffer, size_t len);

/** Prints the buffer as a string of hex characters. */
void hex_print(Print &out, const uint8_t* buffer, size_t len);
