Ascon128 Changelog
==================

1.1.0 - 2026-05-31
------------------

### ⚠️ API Breaking Changes

- To save RAM, `as128_encrypt` and `as128_decrypt` now en-/decrypt the buffer in place instead of to
  a separate output buffer.
- The meaning of the `len` argument to `as128_encrypt` has changed: previously it indicated the size
  of the plaintext excluding the tag, now it indicates the size of the buffer (i.e. 16 bytes more
  than before). This is to bring it in line with `as128_decrypt`.
- `as128_encrypt`, `as128_decrypt`, and `as128_encrypt_print_z85` now all take a `Ascon128 &cipher`
  argument instead of relying on a static variable of their own. Also, they now accept the buffer
  argument as a `void*`.

### Other Changes

- Fixed <https://github.com/rweather/arduinolibs/issues/80> in the local copy, so that now
  `Ascon128::decrypt` can perform in-place decryption as documented.
- `z85_print` was the only remaining function using a `static` variable, but since it was only
  4 bytes, I've now switched to a regular stack variable to make the function reentrant.

1.0.0 - 2026-05-22
------------------

Initial Release.

Earlier Versions
----------------

Development started in the (now archived) <https://github.com/haukex/arduino-ascon128-test>
