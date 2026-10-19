# The Lost Age: field

This folder holds The Lost Age's scene table and, in `common/`, the field
engine code its scenes use: the object script interpreter (`common/script/`),
the map's cells and animation (`common/map/`), field objects
(`common/object/`), events, effects and the camera. In ☀️ the same code is
spread over [Field](../../../tbs/src/field/README.md) and
[Field objects](../../../tbs/src/object/README.md), which explain how it
works; this page says only what differs here.

## What differs from ☀️

- **Grouping.** ⚓️'s folder is by family of the engine, not by ☀️'s objects:
  the script interpreter, the map code and the field objects are subfolders
  of `common/`, one function or small family to a file. `work9c/` and
  `worka0/` are named for the offset of the heap slot their code works in
  (0x9c and 0xa0 in the slot table, slots 39 and 40), and their files and
  functions for their addresses (`Func_080d8d40`), until their meaning is
  read.
- **The scene table.** `scenes.s` has a sixth column, a flag, that ☀️'s
  table does not have, and ⚓️'s scenes run overlays numbered from 0x649
  (`scene Title, 0x649, ...`), where ☀️'s start at 0x36f.
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`.
- **Names.** A function that does what its ☀️ twin does takes its name, so
  the command protocol and the map's words in ☀️'s guides apply here as
  written.

## Status

Functions not yet matched are drafts under `recon/tla/en/main`, by address.
`make progress` gives the counts.

## Evidence and open questions

- XXX: what the scene table's sixth column means.
- XXX: what the work blocks in heap slots 39 and 40 hold, and what their
  code does.
