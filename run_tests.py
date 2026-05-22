#!/usr/bin/env python
# pylint: disable=missing-function-docstring
"""Test runner for Ascon-128.

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
from pathlib import Path
from time import monotonic
from subprocess import run
# https://github.com/pyserial/pyserial
import serial  # type: ignore[import-untyped]

# spell: ignore baudrate FQBN listall

MY_PATH = Path(__file__).parent


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('port', help="The serial port to use")
    parser.add_argument('FQBN', help="The board's FQBN"
                        " - Hint: `arduino-cli board listall`")
    args = parser.parse_args()

    # generate a ZIP archive from this repository
    run(['git', 'archive', '--format', 'zip', '--prefix', 'Ascon128/',
         '--output', 'Ascon128.zip', 'HEAD'], cwd=MY_PATH, check=True)
    # get previous enable_unsafe_install value
    rv = run(['arduino-cli', 'config', 'get', 'library.enable_unsafe_install'],
             capture_output=True, check=True)
    if rv.stderr:
        raise RuntimeError(repr(rv.stderr))
    prev_unsafe = rv.stdout.strip()
    # turn on enable_unsafe_install
    run(['arduino-cli', 'config', 'set', 'library.enable_unsafe_install',
         'true'], check=True)
    # reinstall the library from the ZIP file
    run(['arduino-cli', 'lib', 'uninstall', 'Ascon128'], check=True)
    run(['arduino-cli', 'lib', 'install', '--zip-path', 'Ascon128.zip'],
        cwd=MY_PATH, check=True)
    # restore previous enable_unsafe_install setting
    run(['arduino-cli', 'config', 'set', 'library.enable_unsafe_install',
         prev_unsafe], check=True)

    run(['arduino-cli', 'compile', '--fqbn', args.FQBN, '--warnings', 'all',
         '--upload', '--port', args.port, '--verify',
         MY_PATH/'examples'/'Ascon128-Util-Tests'], check=True)

    run(['python', MY_PATH/'examples'/'Ascon128-Util-Tests'/'test.py',
         args.port], check=True)

    run(['arduino-cli', 'compile', '--fqbn', args.FQBN, '--warnings', 'all',
         '--upload', '--port', args.port, '--verify',
         MY_PATH/'examples'/'TestAscon'], check=True)

    with serial.Serial(port=args.port, baudrate=115200, timeout=10) as ser:
        print("Waiting for boot...")
        start_time = monotonic()
        while True:
            line = ser.readline().rstrip(b'\r\n\0')
            if line:
                print(line.decode('ASCII', 'replace'))
            if line == b'DONE':
                break
            if b'fail' in line.lower():
                raise RuntimeError(line)
            if monotonic() - start_time > 30:
                raise TimeoutError()


if __name__ == '__main__':
    main()
