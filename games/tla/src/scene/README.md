# The Lost Age: scene

This folder holds The Lost Age's scenes: one folder per place, 106 of them,
each an overlay loaded at 0x2008000 like ☀️'s. ☀️'s
[Scenes](../../../tbs/src/scene/README.md) explains how a scene runs, its
tables and its events; this page says only what differs here.

## What differs from ☀️

- **A scene is a folder, not a file.** ☀️ writes each scene as one
  `<place>.c` with its entry and import tables beside it. ⚓️'s scene is a
  folder: `entry.s`, `import.s`, `scene_tables.c`, `data.c`, and its code in
  files named for the offset in the overlay where each function begins
  (`airs_rock_inside/s0128.c`), with a header of its own names where it has
  any (`airs_rock_inside.h`).
- **Linking.** Each overlay is linked by its own script,
  `recon/tla/raw/overlays/resource_<id>.ld`, which lists the folder's objects
  in ROM order and, between them, the sections of
  `resource_<id>_overlay.s` that are not yet C. ☀️ links all its overlays
  through one `overlay.ld` and its object list.
- **Numbers.** ⚓️'s overlays are numbered from 0x649
  (`games/tla/src/field/scenes.s`), and its scene table has a sixth column, a
  flag. A place both games have is a separate ⚓️ scene with its own number
  and map.
- **Engine calls.** The main-image code a ⚓️ scene imports is in
  `games/tla/src/field/common/` and the other ⚓️ folders, under the names of
  its ☀️ twins where it does the same.
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`.

## Status

Functions not yet matched are drafts under `recon/tla/en/overlays`, by
overlay and address (`resource_64b_c_02002594.c`). `make progress` gives the
counts.

## Evidence and open questions

- XXX: which functions several ⚓️ overlays carry as copies, as ☀️'s do in
  its `common/` headers; ⚓️ has no such headers yet.
- XXX: what the scene table's sixth column means.
