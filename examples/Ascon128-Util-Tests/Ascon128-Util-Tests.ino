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
#include <Arduino.h>
#include "hex.hpp"
#include "z85.hpp"
#include "as128.hpp"
#include "uint128.hpp"
#include "as128z85.hpp"

/* Analyzing memory usage on AVR:
 * - In the Arduino IDE, Sketch -> Export Compiled Binary
 *   (ELF file gets written to "build" folder in sketch directory)
 * - Locate the avr-* tools:
 * - `cd "$( arduino-cli config get directories.data )"/packages/arduino/tools/avr-gcc && cd * && cd bin`
 *   (that last bit of the command is written like that to avoid star-slash inside a C comment...)
 * - `avr-size -C <FILE>.elf` - just shows the numbers that the GUI shows too
 * - `avr-nm -S -C -l --format=sysv --size-sort -td <FILE>.elf | grep -E '\.(data|bss|noinit)'`
 * - `avr-objdump -Cwt -j.bss -j.data -j.noinit <FILE>.elf`
 *   - `| perl -wM5.014 -Mvars='$x' -nle '/\.[a-z]+\h+([0-9a-f]+)\h+/i and $x+=hex($1)}{print $x'`
 *
 * (Though both of the latter don't exactly reach the numbers that `avr-size` does.)
 */

constexpr size_t MAIN_BUF_SZ = 320;

/* ********** ********** Hex & Z85 Test ********** ********** */

void hex_test(const uint8_t* buffer, size_t len) {
  static uint8_t out_buf[32];
  while(len) {
    const size_t left = len > 16 ? 16 : len;
    memcpy(out_buf, buffer, left);
    hex_encode(out_buf, left);
    Serial.write(out_buf, left*2);
    #if defined(ARDUINO_ARCH_RENESAS)
      Serial.flush();  // apparently needed on Arduino UNO R4 (bug?)
    #endif
    buffer += left;
    len -= left;
  }
  Serial.println();
}

void z85_test(const uint8_t* buffer, const size_t len) {
  z85_print(Serial, buffer, len);
  Serial.println();
}

/* ********** ********** ********** ********** Ascon-128 Tests ********** ********** ********** ********** */

const uint8_t* SECRET = (uint8_t*)"Super Secret! :)";

Ascon128 cipher;

// Note: In theory, could maybe use `const uint32_t m = millis()` for IV in some cases? Wraps after ~49.7 days!
/** Returns a new IV. */
const uint8_t* next_iv() {
  static uint128_t iv = {0, 0};
  static uint8_t ivb[16];
  memcpy(ivb, &iv, 16);
  iv++;
  return ivb;
}

void as128_enc_z85_test(const uint8_t* buffer, const size_t len) {
  as128_encrypt_print_z85(cipher, Serial, SECRET, next_iv(), buffer, len);
  Serial.println();
}

void as128_enc_test(uint8_t* buffer, const size_t len) {
  if (len+16 > MAIN_BUF_SZ) {
    Serial.println(F("Not enough memory?"));
    return;
  }
  const uint8_t* iv_buf = next_iv();
  hex_print(Serial, iv_buf, 16);
  as128_encrypt(cipher, SECRET, iv_buf, buffer, len+16);
  hex_print(Serial, buffer, len+16);
  Serial.println();
}

void as128_dec_test(uint8_t* buffer, const size_t len) {
  if (!as128_decrypt(cipher, SECRET, buffer, len)) {
    Serial.println(F("Decrypt failed"));
    return;
  }
  hex_print(Serial, buffer, len-16);
  Serial.println();
}

/* ********** ********** ********** ********** Main ********** ********** ********** ********** */

void setup() {
  Serial.begin(115200);
  Serial.println(F("Ready"));
}

void loop() {
  while (!Serial.available());  // block until we get activity on the serial port

  static uint8_t buffer[MAIN_BUF_SZ];
  // readBytesUntil does not include the \n in the resulting buffer.
  const size_t buf_len = Serial.readBytesUntil('\n', buffer, MAIN_BUF_SZ-1);
  if (!buf_len) return;
  if (!hex_decode(buffer+1, buf_len-1)) {
    Serial.println(F("Hex decode failed"));
    return;
  }

  switch(buffer[0]) {
    case 'h':
      hex_test(buffer+1, (buf_len-1)/2);
      break;
    case 'z':
      z85_test(buffer+1, (buf_len-1)/2);
      break;
    case 'c':
      as128_enc_z85_test(buffer+1, (buf_len-1)/2);
      break;
    case 'e':
      as128_enc_test(buffer+1, (buf_len-1)/2);
      break;
    case 'd':
      as128_dec_test(buffer+1, (buf_len-1)/2);
      break;
    default:
      Serial.print(F("0x"));
      Serial.print(buffer[0], HEX);
      Serial.println(F(": Unrecognized command"));
  }

}

// spell: ignore Renesas