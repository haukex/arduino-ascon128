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

/** Write a byte array to the specified stream using the Z85 encoding, with the extension
 * that, as opposed to the specification, buffers of any length can be encoded.
 * (This is the same as the ``z85encode`` and ``z85decode`` functions provided by Python's
 * ``base64`` module as of Python 3.13.)
 *
 * The number of bytes written to the output is ``len + (len + 3) / 4`` (assuming integer
 * arithmetic).
 *
 * References:
 * - https://rfc.zeromq.org/spec/32/
 * - https://github.com/zeromq/rfc/blob/master/src/spec_32.c
 */
void z85_print(Print &out, const uint8_t* buffer, size_t len);
