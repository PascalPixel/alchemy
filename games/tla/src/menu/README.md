# The Lost Age: menu

This folder holds The Lost Age's field menu and its screens (`ability/`,
`djinn/`, `item/`), its shops (`shop/`), and three overlays that are mostly
menus: the title (`title/`, overlay 0x649), the file select screen (`clear/`,
0x64a) and the Battle Arena's lobby (`lobby/`, 0x6b5). ☀️'s
[Menus](../../../tbs/src/menu/README.md) and
[Shops](../../../tbs/src/shop/README.md) explain how these work; this page
says only what differs here.

## What differs from ☀️

- **Grouping.** ⚓️ keeps its shops under `menu/shop/`, where ☀️ has a shop
  folder of its own; the rest follows ☀️'s screens. Each overlay is a folder
  with its entry and import tables, its scene tables and its code, and is
  linked by its own script under `recon/tla/raw/overlays`.
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`.
- **Layout.** One function or small family to a file; in the overlays
  several files are still named by their offset in the overlay
  (`clear/s005c.c`), and in the main image by address (`shop/f10a960.c`).
- **Names.** A function that does what its ☀️ twin does takes its name, so
  ☀️'s guides apply to it as written.

## Status

Functions not yet matched are drafts under `recon/tla/en/main` (main image)
and `recon/tla/en/overlays` (overlays), by address. `make progress` gives the
counts.
