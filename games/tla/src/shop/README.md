# The Lost Age: shop

The Lost Age's shop bank: the counters of its shops, sanctums and inns, and
the forge. [Shops](../../../tbs/src/shop/README.md) explains how they work in
The Broken Seal.

## What differs from The Broken Seal

- The bank also holds the forge (`forge.c`, `forge.tsv`), which The Broken
  Seal does not have: its recipes, the materials the party holds and the roll
  for the result.
- One function or small family goes to a file, some still named for their
  address (`f10a960.c`).
