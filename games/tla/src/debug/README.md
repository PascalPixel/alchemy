# The Lost Age: debug

The Lost Age's debug scenes are scene [overlays](../../../tbs/glossary.md#overlay) built like every
other [scene](../../../tbs/glossary.md#scene), as [Debug scenes](../../../tbs/src/debug/README.md)
describes for The Broken Seal.

## What differs from The Broken Seal

- There are five overlays, where The Broken Seal has three: `rooms/`,
  `level/`, `psynergy_test/`, `psynergy_test_2/` and `shop_test/`. Each is a
  folder holding its [entry table](../../../tbs/glossary.md#entry-table-and-import-table)
  (`entry.s`), its import table (`import.s`), its scene tables and its code;
  the battle's debug parameter sets and the field sequence's `stream.c` are
  in their banks' folders.
- An overlay's code is split into files by function, some still named by
  offset (`level/s004c.c`), and linked as
  [The Lost Age: scene](../scene/README.md) describes.
