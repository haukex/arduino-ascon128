#!/usr/bin/env python
# pylint: disable=missing-function-docstring
"""Test of Ascon-128 on Arduino.

Copyright (c) 2026 Hauke Daempfling at the IGB Berlin

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
"""
import argparse
from itertools import product
from base64 import z85encode, z85decode
# https://github.com/pyserial/pyserial
import serial  # type: ignore[import-untyped]
# https://github.com/meichlseder/pyascon
import ascon   # type: ignore[import-untyped]

# spell: ignore baudrate


def do_hex_test(ser: serial.Serial):
    for t in ('aB', '0123', 'dEadBeeF'):
        ser.write(b'h' + t.encode('ASCII') + b'\n')
        rx = ser.readline().rstrip(b'\r\n')
        exp = t.lower().encode('ASCII')
        if rx != exp:
            raise RuntimeError(f"{t=}: {rx=} != {exp=}")
        print(f"OK {t} {rx.decode('ASCII')}")


def do_z85_test(ser: serial.Serial):
    for length in range(6):
        for combo in product((0, 1, 128, 255), repeat=length):
            t = bytes(combo)
            # Send
            ser.write(b'z' + t.hex().encode('ASCII') + b'\n')
            # Receive
            rx = ser.readline().rstrip(b'\r\n')
            exp = z85encode(t)
            if rx != exp:
                raise RuntimeError(f"{t=}: {rx=} != {exp=}")
            # Test statement from documentation `len + (len + 3) / 4`
            exp_len = len(t) + (len(t) + 3) // 4
            if len(rx) != exp_len:
                raise RuntimeError(f"{len(rx)} != {exp_len}")
            print(f"OK {t.hex()} {rx.decode('ASCII')}")


CRYPT_KEY = b'Super Secret! :)'  # must be 16 bytes

CRYPT_TESTS = (
    b'',
    b'a',
    b'ab',
    b'abc',
    b'abcd',
    b'\x00\x09\x0d\x0a\x18\x7F\x8A\xAB\xFF',
    b'Hello, World! ab',  # 16 bytes
    b'Hello, World! This is a test.',
    # spell: disable
    b'Lorem ipsum dolor sit amet, consectetur adipiscing elit sapien.',  # 63
    # decrypt test: 126*2 + IV as hex 32 + tag as hex 32 +\n+\0 = 318 buffer
    b'Lorem ipsum dolor sit amet, consectetur \xE4dipiscing elit. Pellentesque'
    b' mi magna, pulvinar non \xEEpsum eget, finibus \xE6lquet metus.',  # 126
    # spell: enable
)

_prev_iv: int = -1  # pylint: disable=invalid-name


def _check_enc(t: bytes, rx: bytes, iv: bytes, got: bytes, tag: bytes):
    global _prev_iv  # pylint: disable=global-statement
    # Check IV
    ivn = int.from_bytes(iv, byteorder='little')
    if ivn <= _prev_iv:
        raise ValueError(f"IV repeated: {iv!r}")
    _prev_iv = ivn
    # Decrypt
    out = ascon.decrypt(CRYPT_KEY, iv, iv, got+tag)
    if out != t:  # mismatch
        print(f"##### {t!r} #####")
        print(f" {rx=} iv={ivn!r}")
        # Show expected encryption result
        enc = ascon.encrypt(CRYPT_KEY, iv, iv, t)
        exp = z85encode(iv) + z85encode(enc[:-16]) + z85encode(enc[-16:])
        print(f"{exp=}")
        print(f"    {enc[:-16]!r} tag={enc[-16:]!r}")
        print(f"{got=} {tag=}")
        print(f"{out=}")
        raise RuntimeError(f"{t=} != {out=}")
    print(f"OK {t.decode('CP1252')!r} {rx.decode('ASCII')}")


def do_encrypt_test(ser: serial.Serial):
    for t in CRYPT_TESTS:
        ser.write(b'e' + t.hex().encode('ASCII') + b'\n')
        rx = ser.readline().rstrip(b'\r\n')
        if len(rx) < 64:
            raise ValueError(f"data too short: {rx!r}")
        _check_enc(t, rx, bytes.fromhex(rx[:32]), bytes.fromhex(rx[32:-32]),
                   bytes.fromhex(rx[-32:]))


def do_encrypt_z85_test(ser: serial.Serial):
    for t in CRYPT_TESTS:
        ser.write(b'c' + t.hex().encode('ASCII') + b'\n')
        rx = ser.readline().rstrip(b'\r\n')
        if len(rx) < 40:
            raise ValueError(f"data too short: {rx!r}")
        # Test statement from documentation `len + 32 + (len + 35) / 4`
        exp_len = len(t) + 32 + (len(t) + 35) // 4
        if len(rx) != exp_len:
            raise RuntimeError(f"{len(rx)} != {exp_len}")
        _check_enc(t, rx, z85decode(rx[:20]), z85decode(rx[20:-20]),
                   z85decode(rx[-20:]))


def do_decrypt_test(ser: serial.Serial):
    global _prev_iv  # pylint: disable=global-statement
    for t in CRYPT_TESTS:
        _prev_iv += 1
        iv = _prev_iv.to_bytes(16, byteorder='little')
        enc = iv + ascon.encrypt(CRYPT_KEY, iv, iv, t)
        # Send
        ser.write(b'd' + enc.hex().encode('ASCII') + b'\n')
        # Receive
        rx = ser.readline().rstrip(b'\r\n')
        exp = (iv+t).hex().encode('ASCII')
        if rx != exp:
            raise RuntimeError(f"{t=}: {rx=} != {exp=}")
        print(f"OK {t.decode('CP1252')!r} {enc.hex()}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('port', help="The serial port to use")
    parser.add_argument('-W', '--no-wait', help="don't wait for boot message",
                        action="store_true")
    args = parser.parse_args()
    with serial.Serial(port=args.port, baudrate=115200, timeout=5) as ser:
        if not args.no_wait:
            print("Waiting for boot...")
            if not ser.read_until(b'Ready\r\n').endswith(b'Ready\r\n'):
                raise RuntimeError('Failed to get "Ready" from Arduino')
        do_hex_test(ser)
        do_z85_test(ser)
        do_encrypt_test(ser)
        do_encrypt_z85_test(ser)
        do_decrypt_test(ser)


if __name__ == '__main__':
    main()
