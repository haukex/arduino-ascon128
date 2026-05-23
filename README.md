Ascon-128 and Utility Functions for Arduino
===========================================

This library implements Ascon-128, hexadecimal en-/decoding, and Z85 encoding utility functions.

Entry points:
- For memory-constrained systems, [`as128z85.hpp`](./src/as128z85.hpp) provides
  `as128_encrypt_print_z85`, which encrypts a buffer and immediately prints it to an output stream
  such as `Serial` using the Z85 encoding, so that the encrypted buffer doesn't have to be kept in
  RAM.
- [`as128.hpp`](./src/as128.hpp) provides simple wrappers for the `Ascon128` class that encrypt a
  buffer and use the 16-byte IV as the unencrypted-but-checksummed "associated data".
- [`hex.hpp`](./src/hex.hpp) provides several functions to en- and decode hexadecimal strings
  in-place (or printed directly to an output stream).
- [`uint128.hpp`](./src/uint128.hpp) provides a simple 128-bit integer that can be used as a
  nonce/IV (monotonically increasing counter).
- And of course you are welcome to use only the [Z85](./src/z85.hpp) or
  [Ascon128](./src/Ascon128.h) functions individually.

The code in this repository was tested via `run_tests.py` on the following:
- Arduino UNO R3 (ATmega328P, `arduino:avr:uno`)
- D1 mini (ESP8266, `esp8266:esp8266:d1_mini` / `esp8266:esp8266:d1_mini_clone`)
  - Manually press the reset button immediately after the message "Waiting for boot..."
- Arduino UNO R4 WiFi (`arduino:renesas_uno:unor4wifi`)
  - Manually press the reset button on the message "Done in X seconds" and release on
    "Waiting for boot..."

References:
- https://ascon.isec.tugraz.at/
- https://github.com/meichlseder/pyascon
- https://rweather.github.io/arduinolibs/classAscon128.html
- https://github.com/rweather/arduinolibs/tree/master/libraries/CryptoLW

The reason I split `Ascon128` out of Rhys Weatherley's CryptoLW and Crypto libraries and copied it
here is that the former currently isn't listed as part of the Arduino library collection, making
installation a little trickier, and the latter includes quite a few code files that are not
necessary for Ascon128, some of which cause compilation warnings. Plus, its RNG class sets up a WDT
ISR vector, which interferes with a custom WDT ISR when RNG is not needed.


Author, Copyright, and License
------------------------------

As mentioned in the header of each source file, parts of this software were derived from:
https://github.com/rweather/arduinolibs/tree/37a76b8f7516568e1c575b6dc9268da1ccaac6b6

**Copyright (C) 2015, 2018 Southern Storm Software, Pty Ltd.**

While other parts are:

**Copyright (c) 2026 Hauke Daempfling at the IGB Berlin**

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.