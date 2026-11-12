# The Lost Age: debug

The Lost Age's debug scenes are scene [overlays](../../../tbs/glossary.md#overlay) built like every
other [scene](../../../tbs/glossary.md#scene), as [Debug scenes](../../../tbs/src/debug/README.md)
describes for The Broken Seal.

## What differs from The Broken Seal

- There are five overlays, where The Broken Seal has three:
  `test_scenes.c` (its folder is `rooms/`), `level.c`, `psynergy_test.c`,
  `psynergy_test_2.c` and `shop_test.c`. Each file holds its overlay's scene
  tables and code; beside it, a folder holds its
  [entry table](../../../tbs/glossary.md#entry-table-and-import-table)
  (`entry.s`), its import table (`import.s`) and the code not yet gathered
  into the file. The battle's debug parameter sets and the field sequence's
  stream readers are in their banks' folders.
- Code not yet gathered is named by offset (`psynergy_test/s0c4c.c`) and linked as
  [The Lost Age: scene](../scene/README.md) describes.
