# The Lost Age: scene

The Lost Age's [scenes](../../../tbs/glossary.md#scene), one folder per scene
[overlay](../../../tbs/glossary.md#overlay), 109 of them, each loaded at the same address as The
Broken Seal's. [Scenes](../../../tbs/src/scene/README.md) explains how a scene
runs, its tables and its [events](../../../tbs/glossary.md#event).

## What differs from The Broken Seal

- A scene is one `<scene>.c`, as The Broken Seal's are: its code, then its
  tables. Its [entry table](../../../tbs/glossary.md#entry-table-and-import-table)
  is the one every scene shares, `entry.s`, or its own in `entry/` where its
  routines are named otherwise (the Title, the lobby and three more). Its
  [import table](../../../tbs/glossary.md#entry-table-and-import-table) is
  written by the build: `../system/overlay/imports.inc` lists every far-call
  stub an overlay may take, and each overlay gets the ones its code calls.
  A routine of the field that Camelot linked into the middle of a scene
  (`field/field_effect/move.c` and a few more) is included at its place in the
  scene's file; a scene's tables that the overlay places after a picture
  (the tile pictures every overlay of a dungeon shares) are marked
  `LATE_DATA`, which the overlay's linker script places after it. Where the
  parts a scene was joined from still disagree about a type, each keeps its
  own view of the name, marked `VIEW(part)`, after a `// ---- from ... ----`
  line.
- Both games link every overlay through one `overlay.ld`, one block per
  overlay: an overlay's block names its folder's objects in ROM order, and
  where its globals are still written in assembly they are
  `bss/<scene>.s`, as in The Broken Seal.
- The scene table is in the field folder, with what differs in it
  ([The Lost Age: field](../field/README.md)).
- The main-image code a scene imports is in the bank folders of this game,
  most of it under `games/tla/src/field/`. Code that only overlays use sits
  there too: compiled inside each scene that uses it, which includes it, or its
  own object where an overlay still links it so. `scene/shared/` holds such
  pieces: `drift_spawn.c`, compiled inside Idejima and inside the Shaman Village,
  `scene_set_arrival_flags.c`, compiled inside each scene that
  links it, and the routines several scenes copy, each written once as a macro
  a scene expands where its copy sits, in headers by kind: `cracked_tile.h`,
  `whirlwind.h`, `push.h`, `map_cells.h` and the rest.
- The Title (`title/`), the scene in the row after it, `SceneId_Ov64a`
  (`clear/`, `0x64a`), with its password grid and button codes, and the lobby
  where a link battle is arranged (`lobby/`, `0x6b5`) are overlays here, as
  The Broken Seal's Title, File Screen and Battle Arena are.
